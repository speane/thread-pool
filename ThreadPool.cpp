#include "stdafx.h"
#include "windows.h"
#include "vector"
#include "queue"
#include "mutex"
#include "string"
#include "iostream"
#include "sstream"

class ThreadPool {
public:
	ThreadPool(int count) {
		if (count < 0) {
			logWarn("Thread pool size < 0");
			threadCount = DEFAULT_THREAD_COUNT;
		}
		else {
			threadCount = count;
		}

		HANDLE tempThread;
		for (int i = 0; i < threadCount; i++) {
			tempThread = CreateThread(NULL, 0, threadWork, static_cast<LPVOID>(this), 0, NULL);
			threads.push_back(tempThread);
		}

		closed = false;

		logInfo(threadCount + " threads started");
	}

	void addTask(int taskNumber) {
		taskSyncMutex.lock();

		LPTHREAD_START_ROUTINE task = getTask(taskNumber);
		tasks.push(task);

		taskSyncMutex.unlock();
	}

	void dispose() {
		closed = true;
		WaitForMultipleObjects(threadCount, &threads[0], true, INFINITE);
	}

private:
	const int DEFAULT_THREAD_COUNT = 1;

	static int threadCount;
	static std::vector<HANDLE> threads;
	static std::queue<LPTHREAD_START_ROUTINE> tasks;
	static std::mutex taskSyncMutex;
	static bool closed;
	
	static DWORD WINAPI threadWork(LPVOID lpParam) {
		logInfo("start thread " + getThreadIdString());

		LPTHREAD_START_ROUTINE currentTask;
		while (true) {
			currentTask = getNextTask();
			if (currentTask != NULL) {
				currentTask(lpParam);
			}
			else {
				Sleep(100);
			}
		}
	}

	static LPTHREAD_START_ROUTINE getNextTask() {
		taskSyncMutex.lock();

		LPTHREAD_START_ROUTINE task;
		if (!tasks.empty()) {
			task = tasks.front();
			tasks.pop();
		}
		else {
			task = NULL;
		}

		return task;

		taskSyncMutex.unlock();
	}

	static std::string getThreadIdString() {
		std::stringstream stringStream;
		stringStream << std::this_thread::get_id();
		std::string mystring = stringStream.str();
	}

	static DWORD WINAPI task1(LPVOID lpParam) {
		Sleep(1000);
		logInfo("task1 executed");
	}

	static DWORD WINAPI task2(LPVOID lpParam) {
		Sleep(2000);
		logInfo("task2 executed");
	}

	static DWORD WINAPI task3(LPVOID lpParam) {
		Sleep(3000);
		logInfo("task3 executed");
	}
	
	LPTHREAD_START_ROUTINE getTask(int taskNumber) {
		switch (taskNumber) {
		case 1:
			return task1;
		case 2:
			return task2;
		case 3:
			return task3;
		default:
			return task1;
		}
	}

	static void logInfo(std::string message) {
		std::cout << "[INFO]: " << message;
	}

	static void logWarn(std::string message) {
		std::cout << "[WARN]: " << message;
	}

	static void logError(std::string message) {
		std::cout << "[ERROR]: " << message;
	}
};

int main()
{
	int threadPoolSize;

	std::cout << "Please, enter thread pool size: ";
	std::cin >> threadPoolSize;

	ThreadPool* threadPool = new ThreadPool(threadPoolSize);

	int taskNumber;
	std::cout << "Please choose task to execute(1 - task1; 2 - task2; 3 - task3; 0 - exit)";
	std::cin >> taskNumber;
	while (taskNumber > 0 && taskNumber < 3) {
		threadPool->addTask(taskNumber);
	}
}

