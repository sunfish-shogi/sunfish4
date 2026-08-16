# Sunfish4 の ShogiHome 向け WebAssembly 対応計画

## 1. 結論

次の構成を推奨する。

1. **wasm では pthread を使用しない。** `Threads` オプションを公開せず、探索木を常に
   1 個だけ生成する。
2. **`src/usi` はネイティブ版として維持し、`src/wasm` に新しいエントリーポイントを
   作る。** 盤面構築、時間配分、USI の出力整形など、安定して切り出せる処理だけを
   後から小さな共通部品にする。
3. `usi_command()` でコマンドを受け、`usi_poll()` で反復深化を 1 反復ずつ進める。
   最初の成果物ではこの粒度で適合性を確認し、長い 1 反復のため `stop` の応答性が
   不十分なら、探索内部の再開可能化を別フェーズで行う。
4. ビルドは `emscripten/emsdk:6.0.6` に固定した Docker コマンドだけを正式な手順にし、
   pthread 関係のフラグを一切渡さない。

`src/usi` を wasm に兼用するより新規エントリーポイントの方が、ネイティブ USI の
挙動を壊すリスクと条件コンパイルを大幅に減らせる。探索本体には単一スレッド用の
小さな API 追加が必要だが、通信モデルまで共通化しない方が総変更量は小さい。

## 2. 現状と ABI との差分

### スレッド

- `src/Common.cmake` は非 Windows ビルドへ常に `-pthread` を追加する。
- `Searcher::idsearch()` は `numberOfThreads - 1` 本の `std::thread` を作り、`Tree` 自体も
  `std::thread` をメンバーに持つ。実行時にスレッド数を 1 にするだけでは、pthread
  なしの成果物であることを保証できない。
- `UsiClient` は標準入力の受信スレッドと探索スレッドを別々に作る。さらに
  `ScopedThread`、mutex、atomic、ポーリング待ちを前提とする。
- ShogiHome の ABI は非 shared memory の wasm を要求するため、`-pthread`、
  `-sPROXY_TO_PTHREAD` のどちらも使用できない。

### 実行モデル

現在の `Searcher::idsearch()` は全反復を同期的に完了する API である。単に
`postMessage("go ...")` から呼ぶと Worker のイベントループを占有し、`stop` と
`go infinite` を実装できない。ShogiHome が任意に呼べる `usi_poll()` へ制御を戻すには、
探索セッションの初期化、1 反復の実行、結果確定を分離する必要がある。

### コマンドとデータ

- 現在の `UsiClient` は `isready`、`usinewgame`、`position`、`go` が特定順序で来ることを
  前提とし、不明な入力の多くで `exit()` する。wasm では `quit` 後の出力禁止も状態として
  管理する必要がある。
- `go mate` は現在 `checkmate nomate` を返すが、未対応の場合の ABI 指定値は
  `checkmate notimplemented` である。
- `setoption` の `USI_Hash` は例外を投げ得る `std::stoi` を使う。wasm の Release 設定では
  例外捕捉を無効にするため、全数値入力を非例外の範囲検査付きパーサーへ置き換える。
- 評価値はカレントディレクトリの `eval.bin`、定跡は `book.bin` を読む。配布物では
  マニフェストの `dataFiles` で仮想 FS に配置し、絶対パスまたは注入したパスを読む必要が
  ある。特に `eval.bin` の実サイズを確認し、ShogiHome の 8 MB 警告と起動タイムアウトも
  配布前に評価する。

## 3. 選択肢の比較

### 3.1 pthread を使わない場合

#### Pros

- GitHub Pages のまま動作し、ShogiHome に COOP/COEP、`SharedArrayBuffer`、pthread Worker
  配信の対応を要求しない。
- ABI の適合条件を満たし、通常のブラウザ、PWA、Node の同じ成果物をテストできる。
- 並列探索を当面提供しないという製品方針と一致し、wasm のメモリ使用量も予測しやすい。
- 受信・探索の二重スレッド構成を状態機械へ変えることで、`quit`、`terminate`、`stop` の
  ライフサイクルが明示的になる。

#### Cons

- `-pthread` を消すだけでは足りず、`Tree`、`Searcher`、CMake からスレッド生成コードを
  コンパイル対象外にする必要がある。
- 最も大きい変更は探索の分割実行である。反復深化 1 回単位なら中規模だが、再帰探索を
  数 ms の任意地点で停止・再開する完全な継続状態化は大規模になる。
- 深い反復の途中では JS に戻れないため、1 反復単位の初期実装では `stop` が遅れる可能性が
  ある。Asyncify は改造を減らせる一方、出力サイズと探索速度への悪影響が大きく、正式版の
  第一選択にはしない。

#### 変更量の見積もり

| 項目 | 見積もり | 内容 |
| --- | ---: | --- |
| ビルドとスレッド除去 | 小 (3～5 ファイル) | `Common.cmake` の wasm 分岐、`Tree` と `Searcher` の条件コンパイル、スレッド数 1 の固定 |
| wasm USI アダプター | 中 (新規 4～7 ファイル) | 状態機械、コマンド解析、ABI export、出力、shim、manifest |
| 1 反復単位の探索 API | 中 (主に 2～4 ファイル) | start / poll-one-depth / stop / finish とセッション状態 |
| データと Docker ビルド | 小～中 (3～6 ファイル) | CMake、Docker 実行スクリプト、manifest、評価値・定跡の配置 |
| 任意地点で再開可能な探索 | 大 (探索中核の広範囲) | 再帰スタック、aspiration 再探索、root move の継続状態化。初回スコープ外 |

したがって、**適合性を狙う第一段階は概ね 12～22 ファイル、実装・テスト 5～10 人日**を
目安とする。深い反復中も厳密に短時間で `stop` させる完全な分割実行は、追加で
**10～20 人日以上**を見込む。これは調査時点の構造に基づく幅のある見積もりであり、
Emscripten 6.0.6 での実測後に更新する。

### 3.2 `src/usi` を wasm 対応させる場合

#### Pros

- USI オプション、時刻計算、定跡、info/bestmove の整形を二重実装せずに済む。
- 将来のプロトコル修正がネイティブ版と wasm 版へ同時に反映される。

#### Cons

- `UsiClient` の中心設計が「stdin 受信スレッド + 探索スレッド + ブロッキング状態遷移」なので、
  callback + poll 型に抽象化するには実質的な再設計になる。
- 多数の `#ifdef __EMSCRIPTEN__`、または入力・スケジューラ・終了方法の抽象クラスが必要で、
  十年以上動いているネイティブ経路の回帰範囲が広がる。
- `exit()`、待機ループ、queue/mutex、ponder の前提が wasm のライフサイクルと衝突する。
- 一見したコード共有率ほど工数を節約できず、wasm 固有の単一スレッド状態機械が読みにくくなる。

### 3.3 `src/wasm` に新規エントリーポイントを作る場合（推奨）

#### Pros

- pthread を参照しないことをターゲット単位で検証でき、`src/usi` のネイティブ挙動を維持できる。
- `usi_command`、`usi_poll`、`terminate` 相当の状態を ABI に沿って直接設計できる。
- ponder と `Threads` を最初から除外でき、必要なコマンドだけの小さな状態機械にできる。
- wasm のパス、メモリ上限、入力検証、終了後の出力抑止を局所化できる。

#### Cons

- 時刻計算、USI オプション、info 出力などに当初は重複が生じる。
- ネイティブ版と wasm 版でオプション定義がずれる可能性があるため、manifest と実際の
  `option` 行をテストで照合する必要がある。
- バグ修正を両方へ適用する場面がある。

#### 総合判断

新規エントリーポイントを採用する。共通化は先に大きな抽象化を作らず、次の純粋処理に限定して
重複が実際に発生した時点で `src/usi/common` 等へ抽出する。

- `go` の時間引数から `SearchConfig` を作る処理
- score、PV、探索統計から `info` 行を作る処理
- 範囲検査付き整数パーサー
- `position` コマンドから `Record` を作る既存 `SfenParser`

## 4. 実装フェーズ

### Phase 0: ベースラインと成果物契約

1. ネイティブの `make test` と `make usi` を記録する。
2. 配布する `eval.bin` と任意の `book.bin` の由来、ライセンス、サイズ、チェックサムを確認する。
3. 初期版の定跡は任意とし、ファイルを同梱しない場合は `UseBook` の既定値を false にする。
4. 安定 ID（例: `sunfish4-v1`）、表示名、author、公開オプションを確定する。

### Phase 1: pthread なしで探索ライブラリをビルド

1. `Common.cmake` に Emscripten 分岐を追加し、wasm では `-pthread` と x86 専用フラグを
   付けない。ネイティブ設定は変更しない。
2. `SUNFISH_SINGLE_THREAD` のようなターゲット定義を導入する。
3. その定義下では `Tree::thread` を持たず、`Searcher::idsearch()` の補助スレッド生成・joinを
   コンパイルしない。探索木数を API 内でも 1 に clamp し、設定ミスでスレッド経路へ入れない。
4. wasm リンク結果に shared memory、pthread import、不要な Worker ファイルがないことを
   `wasm-objdump` と成果物一覧で検査する。

atomic と uncontended mutex は直ちに全面除去しない。Emscripten 6.0.6 の非 pthread
ビルドでコンパイル・リンクできるものは残し、実際に pthread 依存を生む箇所だけ条件分岐する。
これにより探索本体の変更を抑える。

### Phase 2: 探索を poll 可能にする

1. `Searcher` に単一スレッド専用の探索セッションを追加する。
   - `startIDSearch(position, maxDepth, record)`：初期化と root move 生成
   - `pollIDSearch()`：次の反復を 1 回実行し、継続中/完了を返す
   - `stopIDSearch()`：停止要求を記録し、現在までの最善手を確定
   - `finishIDSearch()`：結果と統計を確定し、次の探索を可能にする
2. 既存 `idsearch()` はこの内部部品を同期ループで呼ぶ形に寄せ、可能な限り探索ロジックを
   一本化する。ただしネイティブ並列探索の root-worker 制御は既存経路に残す。
3. mate、合法手なし、時間切れ、最大深さ、`go infinite`、`stop` の各終了条件をテストする。
4. 1 回の `pollIDSearch()` の時間を実測する。深い局面で許容値を超える場合は次の順で判断する。
   - conformance と通常対局に十分なら 1 反復粒度を採用
   - 不十分なら root move 単位まで再開状態を追加
   - それでも不十分な場合のみ再帰探索の継続状態化を設計

Asyncify は比較用の実験ターゲットに限る。速度、wasm サイズ、`stop` latency を測り、明確に
優位な場合だけ採用する。

### Phase 3: `src/wasm` の USI 状態機械

1. `WasmUsiClient`（仮称）を作り、公開 C 関数 `usi_command(const char*)` と
   `usi_poll()` を実装する。`main()` と stdin は使用しない。
2. コマンドを最低限次のように扱う。
   - `usi`: name、author、オプション、`usiok`
   - `setoption`: `USI_Hash`、`UseBook`、`Snappy`、`MarginMs`、`MaxDepth`、`MultiPV`
   - `isready`: データを検証・初期化して `readyok`
   - `usinewgame`, `position`, `go`, `stop`, `gameover`
   - `go mate`: `checkmate notimplemented`
   - `quit`: 探索を破棄して永続的な terminated 状態へ移る
3. `Threads` は申告も解釈もしない。`USI_Ponder` は false とし、初期版では ponder を行わない。
4. `go` は探索セッションを開始するだけで戻り、`usi_poll()` が進行させる。完了時に一度だけ
   `bestmove` を出す。`stop` はそれまでの最善手（まだなければ最初の合法手または resign）を
   即座に確定する。
5. 出力は stdout に 1 行ずつ書いて flush する。quit/terminate 後は共通の output gate で
   全出力を捨てる。
6. 空入力、不足引数、不正数値、過大な Hash で abort/範囲外アクセスしないテストを追加する。
   `USI_Hash` はブラウザ向け上限（初案 256 MB、実測後に縮小可）を持たせる。

### Phase 4: Emscripten 6.0.6 の Docker ビルド

`src/wasm/CMakeLists.txt` と、ホスト環境に emsdk を要求しない薄いスクリプト
`tools/build-wasm.sh` を追加する。正式な起動形は次のように固定する。

```sh
docker run --rm \
  -v "$PWD:/src" \
  -w /src \
  emscripten/emsdk:6.0.6 \
  bash tools/build-wasm-in-docker.sh
```

コンテナ内では `emcmake cmake` と `cmake --build` を使い、少なくとも次をリンクする。

```text
-sMODULARIZE=1
-sEXPORT_ES6=1
-sEXPORT_NAME=createSunfish4
-sENVIRONMENT=worker,node
-sINVOKE_RUN=0
-sALLOW_MEMORY_GROWTH=1
-sSTACK_SIZE=1048576
--no-entry
--pre-js src/wasm/shim.js
-sEXPORTED_FUNCTIONS=_usi_command,_usi_poll,_malloc,_free
-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,FS
```

`shim.js` は ABI の参照 shim と同じ契約を実装し、ライセンスと同期元をコメントに残す。
最適化、LTO、初期/最大メモリは機能確認後にベンチマークで決める。`-pthread` はコンパイル・
リンクのどちらにも渡さず、Docker イメージの floating tag は使わない。

### Phase 5: manifest とデータファイル

1. 出力先を `out/wasm/sunfish4/` とし、`engine.json`、`sunfish4.js`、`sunfish4.wasm`、
   必要なデータをそのまま ShogiHome の `public/engines/sunfish4/` へコピーできる形にする。
2. `engine.json` は `abi: shogihome-wasm-engine/1`、`moduleFormat: esm` とし、USI が申告する
   オプションを正確に複製する。
3. `eval.bin` は `dataFiles` で `/sunfish4/eval.bin` に置く。`Evaluator` にはファイルパスを
   注入できる API を追加し、カレントディレクトリ依存をなくす。book を配布する場合も同様にする。
4. データなし・破損・メモリ確保失敗を `isready` の失敗として安全に扱い、タブ全体を abort
   させない。ShogiHome の起動タイムアウト内でロードできるか実測する。

### Phase 6: テストと受け入れ条件

Sunfish4 側に Node の smoke/conformance テストを置き、ShogiHome へコピーした後に本家の
適合性テストも実行する。

受け入れ条件は次の通り。

- Docker のクリーン環境から再現可能に `.js` と `.wasm` が生成される。
- wasm が shared memory を要求せず、pthread 補助 Worker を生成しない。
- `usi` / `isready` / `position` / `go` が動作し、返した手が合法である。
- `go infinite` 後の `stop` が `bestmove` を返し、二重に返さない。
- `go mate` が `checkmate notimplemented` を返す。
- `quit` と `terminate()` の後には一切出力しない。
- manifest の options とエンジン出力が一致する。
- 不正なコマンド引数で wasm が abort しない。
- 既存のネイティブ test と USI build が回帰しない。
- Chrome/Firefox の Worker と Node の双方で起動できる。

## 5. 主なリスクと判断ゲート

| リスク | 対策 / 判断ゲート |
| --- | --- |
| 1 反復が長く `stop` が遅い | Phase 2 で局面別 p50/p95/max を測る。適合性だけでなく対局中断の UX を基準に次の分割粒度を決める |
| 評価ファイルが大きい | サイズ・ライセンスを Phase 0 で確認し、`dataFiles`、Hash 上限、起動 timeout を合わせて測る |
| wasm32 のメモリ不足 | Hash を上限付きにし、初期化を段階化する。OOM を起こす設定を manifest へ出さない |
| ネイティブ探索の回帰 | 新 API の同期 wrapper テストと既存 test を CI で併走させる |
| ABI shim のずれ | ShogiHome の参照 shim と conformance を固定 revision で追跡する |
| Emscripten 更新差 | 正式ビルドは 6.0.6 に固定し、更新は別 PR で成果物差分とベンチマークを確認する |

## 6. 実装 PR の分割案

1. **単一スレッド build seam**: CMake 分岐、探索補助スレッドの除外、ネイティブ回帰テスト。
2. **incremental search API**: start/poll/stop/finish と単体テスト、poll latency の測定結果。
3. **wasm USI adapter**: `src/wasm` の状態機械、入力検証、Node テスト。
4. **Docker/package**: emsdk 6.0.6 ビルド、shim、manifest、データ配置、成果物検査。
5. **ShogiHome integration**: ShogiHome conformance、ブラウザ smoke test、カタログ登録。

PR 2 の計測で 1 反復単位が不十分と判明した場合は、PR 3 より前に「root move 単位の
再開」または「探索スタックの継続状態化」を独立した設計・実装 PR として挿入する。
