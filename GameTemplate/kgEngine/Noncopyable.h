#pragma once
//コピーを禁止するクラスに継承させる、構造体も継承できる、デフォルトではpublic継承らしい
struct Noncopyable{
	Noncopyable() = default;
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;
};

//クラスを定義した際に暗黙的に定義される関数があるが、それらを制御する = default と = delete を使っている
// = defaultは暗黙定義される関数を使用する
// = deleteは暗黙定義される関数の使用を禁止する

