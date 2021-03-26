#include "AudioEffects.h"

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;

}

HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}


AudioEffects::AudioEffects()
{
	pDataBuffer = nullptr;
	AudioVolume.reserve(AudioType_COUNT);
	for (size_t i = 0; i < AudioType_COUNT; i++)
	{
		AudioVolume.emplace_back(1.0f);
	}
}


AudioEffects::~AudioEffects()
{
	for (size_t i = 0; i < pAudioVoices.size(); i++) {
		if (pAudioVoices[i].pAudioVoice)
			pAudioVoices[i].pAudioVoice->Stop(0);
	}
	if (pDataBuffer) {
		delete[] pDataBuffer;
		pDataBuffer = nullptr;
	}
	for (size_t i = 0; i < trackToDelete.size(); i++) {
		delete[] trackToDelete[i];
		trackToDelete[i] = nullptr;
	}
	trackToDelete.clear();

	if (pMasterVoice)
		pMasterVoice->DestroyVoice();
	for (size_t i = 0; i < pAudioVoices.size(); i++) {
		if(pAudioVoices[i].pAudioVoice)
			pAudioVoices[i].pAudioVoice->DestroyVoice();
	}
	if (pXAudio2)
		pXAudio2->Release();

	//pXAudio2->Release();
	//delete pMasterVoice;
	//delete pSourceVoice;
}

HRESULT AudioEffects::InitAudio()
{
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&pXAudio2, NULL, XAUDIO2_DEFAULT_PROCESSOR)))
		return hr;

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;

	return S_OK;
}


HRESULT AudioEffects::LoadAudio(std::wstring strFileName, UINT audioType = -1)
{
	// Open the file
	HANDLE hFile = CreateFile(
		strFileName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	// remove previous memory before allocating new memory to avoid memory leak
	trackToDelete.push_back(pDataBuffer);

	pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);
	if (audioType == AudioType::AudioType_Music) {
		buffer2.LoopCount = XAUDIO2_LOOP_INFINITE;
		buffer2.AudioBytes = dwChunkSize;  //buffer containing audio data
		buffer2.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
		buffer2.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	}
	else {
		buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
		buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
		buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	}

	return S_OK;
}

HRESULT AudioEffects::PlayAudio(UINT SourceVoice, std::wstring strFileName, UINT audioType)
{
	if (pAudioVoices.size() <= SourceVoice) {
		pAudioVoices.resize(SourceVoice + 1);
	}
	if (pAudioVoices[SourceVoice].pAudioVoice) {
		pAudioVoices[SourceVoice].pAudioVoice->DestroyVoice();
		pAudioVoices[SourceVoice].pAudioVoice = nullptr;
	}

	HRESULT hr;
	if (audioType == AudioType::AudioType_Music) {
		if (FAILED(hr = LoadAudio(strFileName, AudioType::AudioType_Music)))
			return hr;

		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pAudioVoices[SourceVoice].pAudioVoice, (WAVEFORMATEX*)&wfx)))
			return hr;

		if (FAILED(hr = pAudioVoices[SourceVoice].pAudioVoice->SubmitSourceBuffer(&buffer2)))
			return hr;

		if (FAILED(hr = pAudioVoices[SourceVoice].pAudioVoice->Start(0)))
			return hr;

	}
	else {
		if (FAILED(hr = LoadAudio(strFileName)))
			return hr;

		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pAudioVoices[SourceVoice].pAudioVoice, (WAVEFORMATEX*)&wfx)))
			return hr;

		if (FAILED(hr = pAudioVoices[SourceVoice].pAudioVoice->SubmitSourceBuffer(&buffer)))
			return hr;

		if (FAILED(hr = pAudioVoices[SourceVoice].pAudioVoice->Start(0)))
			return hr;
	}


	pAudioVoices[SourceVoice].Channel = audioType;
	pAudioVoices[SourceVoice].isStop = false;

	UpdateAudio();

	return S_OK;
}

bool AudioEffects::GetIsStop(UINT SourceVoice) {
	if (pAudioVoices.size() > SourceVoice)
	{
		if (pAudioVoices[SourceVoice].isStop) return true;
		if (pAudioVoices[SourceVoice].pAudioVoice) {
			AudioEffects::AudioChannels tempChan = pAudioVoices[SourceVoice];
			XAUDIO2_VOICE_STATE state;
			tempChan = pAudioVoices[SourceVoice];
			tempChan.pAudioVoice->GetState(&state);
			if (state.SamplesPlayed == 0)
			{
				return true;
			}
		}
		else {
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}
void AudioEffects::DestroyVoice(UINT SourceVoice) {
	if (pAudioVoices.size() <= SourceVoice) {
		return;
	}
	if (pAudioVoices[SourceVoice].pAudioVoice) {
		pAudioVoices[SourceVoice].pAudioVoice->DestroyVoice();
		pAudioVoices[SourceVoice].pAudioVoice = nullptr;
	}
}
HRESULT AudioEffects::StopAudio() {

	for (size_t i = 0; i < pAudioVoices.size(); i++)
	{
		StopAudio(i);
	}

	return S_OK;
}

HRESULT AudioEffects::StopAudio(UINT SourceVoice) {
	if (pAudioVoices.size() > SourceVoice)
	{
		if (pAudioVoices[SourceVoice].pAudioVoice)
		{
			pAudioVoices[SourceVoice].pAudioVoice->Stop(0);
			pAudioVoices[SourceVoice].isStop = true;
		}
			
	}
	

	return S_OK;
}

HRESULT AudioEffects::StartAudio(IXAudio2SourceVoice* pSourceVoice, std::wstring strFileName)
{
	if (pSourceVoice) {
		pSourceVoice->Start(0);
	}
	else {
		HRESULT hr;
		if (FAILED(hr = LoadAudio(strFileName)))
			return hr;

		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
			return hr;

		if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
			return hr;

		if (FAILED(hr = pSourceVoice->Start(0)))
			return hr;
	}

	UpdateAudio();

	return S_OK;
}

HRESULT AudioEffects::UpdateAudio()
{
	for (size_t i = 0; i < pAudioVoices.size(); i++)
	{
		if (pAudioVoices[i].pAudioVoice == nullptr) {
			Dropdownsettings[1] = false;
		}
	}

	for (size_t SourceVoice = 0; SourceVoice < pAudioVoices.size(); SourceVoice++)
	{
		if (pAudioVoices[SourceVoice].Channel == 1) {
			pAudioVoices[SourceVoice].pAudioVoice->SetVolume(AudioVolume[AudioType::AudioType_Music]);
		}
		else if (pAudioVoices[SourceVoice].Channel == 2) {
			if (pAudioVoices[SourceVoice].pAudioVoice)
				pAudioVoices[SourceVoice].pAudioVoice->SetVolume(AudioVolume[AudioType::AudioType_Effects]);
		}
		else if (pAudioVoices[SourceVoice].Channel == 3) {
			pAudioVoices[SourceVoice].pAudioVoice->SetVolume(AudioVolume[AudioType::AudioType_Dialog]);
		}
	}

	pMasterVoice->SetVolume(AudioVolume[AudioType::AudioType_Master]);

	return S_OK;
}






