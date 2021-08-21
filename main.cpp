#include <GLFW/glfw3.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <sndfile.h>

void setupOpenAl(); 
void checkForErrors();

void setupOpenAl() {
	checkForErrors();

	ALenum err, format;
    SF_INFO sfinfo;

	ALboolean enumeration;
	ALCdevice *device;
	const ALCchar *defaultDeviceName;
	ALCcontext* context;
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALuint source;
	ALuint buffer;

	// Check for enumeration extension
	std::cout << "Checking alc extensions" << std::endl;
	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXIT");
	if(enumeration == AL_FALSE) {
		std::cout << "ENUMERATION FALSE";
	} else {
		std::cout << "ENUMERATION TRUE" << std::endl;
	}

	// Find device and set ALC context
	std::cout << "\nSetting up Device\n";
	device = alcOpenDevice(NULL);
	if(!device) {
		std::cout << "FAILURE FINDING AUDIO DEVICE\n";
		return;
	}
	checkForErrors();
	std::cout << "Setting up Device name\n";
	defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	device = alcOpenDevice(defaultDeviceName);

	context = alcCreateContext(device, NULL);
	std::cout << "Setting up Context\n";
	if(!alcMakeContextCurrent(context)) {
		std::cout << "FAILURE SETTING CURRENT CONTEXT\n";
		return;
	}
	checkForErrors();

	// Setup Listeners
	std::cout << "Setting up listeners\n";
	alListener3f(AL_POSITION, 0, 0, 1.0f); // check for errors
	checkForErrors();
	alListener3f(AL_VELOCITY, 0, 0, 0);
	checkForErrors();
	alListenerfv(AL_ORIENTATION, listenerOri);
	checkForErrors();

	// Setup source
	alGenSources((ALuint)1, &source); 
	checkForErrors();
	alSourcef(source, AL_PITCH, 1);
	checkForErrors();
	alSourcef(source, AL_GAIN, 1);
	checkForErrors();
	alSource3f(source, AL_POSITION, 0, 0, 0);
	checkForErrors();
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	checkForErrors();
	alSourcei(source, AL_LOOPING, AL_FALSE);
	checkForErrors();

	// Setup buffers
	alGenBuffers((ALuint)1, &buffer);
	checkForErrors();

	// Playout logic
	const char * fname = "pinkpanther.wav";
	SNDFILE *inFile;
	SF_INFO inFileInfo;
	inFile = sf_open(fname, SFM_READ, &inFileInfo);
	int fs = inFileInfo.samplerate;
	std::cout << "SAMPLE RATE " << fs << std::endl;

	// LibSndFile
	format = AL_NONE;
	if (inFileInfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (inFileInfo.channels == 2)
		format = AL_FORMAT_STEREO16;

	if (!format) {
		std::cout << "FORMAT FAILED" << std::endl;
	} else {
		std::cout << inFileInfo.channels << std::endl;
	}
	
	// Read file data into buffer
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;

	membuf = static_cast<short*>(malloc((size_t)(inFileInfo.frames * inFileInfo.channels) * sizeof(short)));
	num_frames = sf_readf_short(inFile, membuf, inFileInfo.frames);
	num_bytes = (ALsizei)(num_frames * inFileInfo.channels) * (ALsizei)sizeof(short);
	std::cout << "InFileInfo CHANNELS " << inFileInfo.channels << std::endl;
	std::cout << "InFileInfo FRAMES" << inFileInfo.frames << std::endl;
	std::cout << "NUM FRAMES" << num_frames << std::endl;
	std::cout << "NUM BYTES" << num_bytes << std::endl;
	std::cout << "LHS" << inFileInfo.channels << std::endl;

	alBufferData(buffer, format, membuf, num_bytes, inFileInfo.samplerate);
	free(membuf);
	sf_close(inFile);
	checkForErrors();

	ALint source_state;	
	alSourcei(source, AL_BUFFER, buffer);
	checkForErrors();

	alSourcePlay(source);
	checkForErrors();

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	checkForErrors();
	
	std::cout << "PLAYING" << std::endl;
	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		checkForErrors();
	}

	/* cleanup */
	std::cout << "Initiating cleanup" << std::endl;
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	std::cout << "Completed cleanup" << std::endl;
}

void checkForErrors() {
	ALCenum error;
	error = alGetError();
	if(error != AL_NO_ERROR) {
		std::cout << "\nERROR!!!!\n";
	}
	if(error != AL_NO_ERROR)
	{
		switch(error)
		{
			case AL_INVALID_NAME:
				std::cout << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
				break;
			case AL_INVALID_ENUM:
				std::cout << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
				break;
			case AL_INVALID_VALUE:
				std::cout << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
				break;
			case AL_INVALID_OPERATION:
				std::cout << "AL_INVALID_OPERATION: the requested operation is not valid";
				break;
			case AL_OUT_OF_MEMORY:
				std::cout << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
				break;
			default:
				std::cout << "UNKNOWN AL ERROR: " << error;
		}
		std::cout << std::endl;
		return;
	}
}

int main(void)
{
	/* Initialize the library */
	std::thread t1(setupOpenAl);

	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		t1.join();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	t1.join();
	glfwTerminate();
	return 0;
}
