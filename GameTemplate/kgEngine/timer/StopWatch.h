#pragma once
#include <winnt.h>
#include <profileapi.h>
//���Ԍv���A1�t���[���̌o�ߎ��Ԃ��v��̂ɂ��g��
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
	//�v���J�n
	//������ӂȂɂ���Ă邩��������킩��˂���
	void Start()
	{
		//�v���J�n���̃J�E���g�����擾
		QueryPerformanceCounter((LARGE_INTEGER*)& begin);
	}
	//�v���I��
	void Stop()
	{
		//���g�����擾
		::QueryPerformanceFrequency((LARGE_INTEGER*)& freq);
		//�v���I�����̃J�E���g�����擾
		::QueryPerformanceCounter((LARGE_INTEGER*)& end);
		elapsed = double(end - begin) / freq;
		elapsedMill = elapsed * 1000.0;
		elapsedMicro = elapsedMill * 1000.0;

	}
	//�o�ߎ��Ԃ��擾(�b)
	double GetElapsed() const
	{
		return elapsed;
	}
	//�o�ߎ��Ԃ��擾(�~���b)
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
	//�o�ߎ��Ԃ��擾(�}�C�N���b)
	double GetElapsedMicrosecond() const
	{
		return elapsedMicro;
	}
private:
	//�Ȃ�int�^�̗e�ʑ傫����A64�r�b�g�ȏ��\���ł���Ƃ��Ȃ�Ƃ�
	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//�o�ߎ���(�P�ʁF�b)
	double elapsedMill;		//�o�ߎ���(�P�ʁF�~���b)
	double elapsedMicro;	//�o�ߎ���(�P�ʁF�}�C�N���b)
};