/*
Author: Patrick Gelvin
Date: April 10th, 2021
*/

#pragma once
#ifndef _FILEUTILS_H
#define _FILEUTILS_H

#include <filesystem>
#include <optional>
#include <fstream>

std::optional<std::ofstream*> openForWrite(const std::string);
std::optional<std::ifstream*> openForRead(const std::string);

#endif
