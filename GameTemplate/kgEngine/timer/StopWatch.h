#pragma once
#include <winnt.h>
#include <profileapi.h>
//時間計るやつ、1フレームの経過時間を計るのにも使う
class CStopWatch {
public:
	CStopWatch()
	{
		freq = 0;
		end = 0;
		begin = 0;
		elapsed = 0.0;
		elapsedMill = 0.0;
		elapsedMicro = 0.0;
	}
	~CStopWatch()
	{

	}
	//計測開始
	//ここら辺なにやってるかこれもうわかんねぇな
	void Start()
	{
		//計測開始時のカウント数を取得
		QueryPerformanceCounter((LARGE_INTEGER*)& begin);
	}
	//計測終了
	void Stop()
	{
		//周波数を取得
		::QueryPerformanceFrequency((LARGE_INTEGER*)& freq);
		//計測終了時のカウント数を取得
		::QueryPerformanceCounter((LARGE_INTEGER*)& end);
		elapsed = double(end - begin) / freq;
		elapsedMill = elapsed * 1000.0;
		elapsedMicro = elapsedMill * 1000.0;

	}
	//経過時間を取得(秒)
	double GetElapsed() const
	{
		return elapsed;
	}
	//経過時間を取得(ミリ秒)
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
	//経過時間を取得(マイクロ秒)
	double GetElapsedMicrosecond() const
	{
		return elapsedMicro;
	}
private:
	//なんかint型の容量大きいやつ、64ビット以上を表現できるとかなんとか
	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//経過時間(単位：秒)
	double elapsedMill;		//経過時間(単位：ミリ秒)
	double elapsedMicro;	//経過時間(単位：マイクロ秒)
};