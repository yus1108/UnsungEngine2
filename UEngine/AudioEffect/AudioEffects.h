#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <winerror.h>
#include <bitset>
#include <xaudio2.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"xaudio2.lib")

using namespace std;

enum AudioType {
	AudioType_Master = 0,
	AudioType_Music,
	AudioType_Effects,
	AudioType_Dialog,
	AudioType_COUNT
};

class AudioEffects
{
private:
	struct AudioChannels
	{
		IXAudio2SourceVoice* pAudioVoice;
		UINT Channel;
	};

	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
	XAUDIO2_BUFFER buffer2 = { 0 };

	IXAudio2* pXAudio2 = NULL;
	IXAudio2MasteringVoice* pMasterVoice = NULL;
	IXAudio2Voice* pVoice;

	BYTE * pDataBuffer;
	std::vector<BYTE *> trackToDelete;

	const float *pVolumes;

	vector<AudioChannels> pAudioVoices;

	HRESULT LoadAudio(std::wstring strFileName, UINT audioType);
	bool GetIsStop(UINT SourceVoice);
	void DestroyVoice(UINT SourceVoice);
public:
	bitset<2> Dropdownsettings;
	vector<float> AudioVolume;

	AudioEffects();
	~AudioEffects();
	// 0 = background music
	// 1 = Button clicks
	// 2 = Sound Effects from selecting
	// 3 = 

	float firstplay = 0;

	HRESULT InitAudio();
	HRESULT PlayAudio(UINT pSourceVoice, std::wstring strFileName, UINT audioType = 0);
	HRESULT StartAudio(IXAudio2SourceVoice* pSourceVoice, std::wstring strFileName);
	HRESULT StopAudio(UINT SourceVoice);
	HRESULT StopAudio();
	HRESULT UpdateAudio();

};

