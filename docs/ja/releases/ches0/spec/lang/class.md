# クラス

## 構文

```
<クラス名>

<メンバ1>

<メンバ2>

    .
    .
    .
```

### コード例

```
Names  // クラス名

// 静的メンバ変数
str creatorName = "Garnet"

// メンバ変数
str .name = "Micheal"

// 静的メンバ関数
greetCreator()
    println("Hello, " ~ creatorName ~ "!")
end

// メンバ関数
.changeName(str newName)
    .name = newName
end
```

## アクセス修飾子

|修飾子|スコープ|
|:-:|:-:|
|public|全体|
|private|クラス内|

## メンバ

### アクセス修飾子

|修飾子|スコープ|
|:-:|:-:|
|public|全体|
|private|クラス内|

### 静的メンバ

メンバ名の前に `.` (ピリオド) を置いて表す

(例: `int .num = 10`)
