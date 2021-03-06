// day3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <string>
#include <set>

using namespace std;

enum CELL {
	EMPTY,
	ONE_CLAIM,
	MULTIPLE_CLAIMS
};

struct InputEntry {
	int id;
	int startCol, startRow;
	int width, height;
};

bool readEntry(FILE *f, InputEntry &entry) {
	char temp[128];
	bool eof = false;
	if (fgets(temp, 128, f) == nullptr) {
		return false;
	}
	sscanf_s(temp, "#%d @ %d,%d: %dx%d", &entry.id, &entry.startCol, &entry.startRow, &entry.width, &entry.height);
	return true;
}

void fillMatrix(vector<vector<CELL>> &matrix, const InputEntry &entry, int &conflictingFabric) {
	for (int row = entry.startRow; row < entry.startRow + entry.height; row++) {
		for (int col = entry.startCol; col < entry.startCol + entry.width; col++) {
			switch (matrix.at(row).at(col)) {
			case EMPTY:
				matrix.at(row).at(col) = ONE_CLAIM;
				break;
			case ONE_CLAIM:
				matrix.at(row).at(col) = MULTIPLE_CLAIMS;
				conflictingFabric++;
				break;
			case MULTIPLE_CLAIMS:
				break;
			}
		}
	}
}

int readFabric(const string &filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "r+");
	int conflictingFabric = 0;
	InputEntry entry;
	vector<vector<CELL>> matrix (1000, vector<CELL>(1000));
	while (readEntry(f, entry)) {
		fillMatrix(matrix, entry, conflictingFabric);
	}
	fclose(f);
	return conflictingFabric;
}

void fillMatrix(vector<vector<set<int>>> &matrix, const InputEntry &entry) {
	for (int row = entry.startRow; row < entry.startRow + entry.height; row++) {
		for (int col = entry.startCol; col < entry.startCol + entry.width; col++) {
			matrix.at(row).at(col).insert(entry.id);
		}
	}
}

bool isNotOverlapped(const vector<vector<set<int>>> &matrix, const InputEntry &entry) {
	for (int row = entry.startRow; row < entry.startRow + entry.height; row++) {
		for (int col = entry.startCol; col < entry.startCol + entry.width; col++) {
			if (matrix.at(row).at(col).size() > 1) {
				return false;
			}
		}
	}
	return true;
}

int getNonOverlappingId(const vector<vector<set<int>>> &matrix, const vector<InputEntry> &entries) {
	for (const InputEntry &entry : entries) {
		if (isNotOverlapped(matrix, entry)) {
			return entry.id;
		}
	}
	return -1; // should never be reached
}

int readNonOverlappedId(const string &filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "r+");
	int id;
	InputEntry entry;
	vector<vector<set<int>>> matrix(1000, vector<set<int>>(1000));
	vector<InputEntry> entries;
	while (readEntry(f, entry)) {
		fillMatrix(matrix, entry);
		entries.push_back(entry);
	}
	id = getNonOverlappingId(matrix, entries);
	fclose(f);
	return id;
}

int main() {
	int fabric = readFabric("input.txt");
	cout << "Fabric within 2 or more claims: " << fabric << " square inches\n";
	int id = readNonOverlappedId("input.txt");
	cout << "ID not overlapped: " << id << "\n";
}