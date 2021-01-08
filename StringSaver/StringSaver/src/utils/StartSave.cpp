#include "..\Link.h"

std::vector<std::future<void>> vectorThread;
void StartSave() {

	if (ScanJavaw)		vectorThread.emplace_back(std::async(std::launch::async, ProcessSave, "javaw",		false));
	if (ScanExplorer)	vectorThread.emplace_back(std::async(std::launch::async, ProcessSave, "explorer",	false));
	if (ScanDWM)		vectorThread.emplace_back(std::async(std::launch::async, ProcessSave, "dwm",		false));
	if (ScanLsass)		vectorThread.emplace_back(std::async(std::launch::async, ProcessSave, "lsass",		false));
	if (ScanDPS)		vectorThread.emplace_back(std::async(std::launch::async, ProcessSave, "DPS",		true));
	if (ScanPcaSvc)		vectorThread.emplace_back(std::async(std::launch::async, ProcessSave, "PcaSvc",		true));

	for (auto&& task : vectorThread) {
		task.get();
	}

	vectorThread.clear();

	return;
}