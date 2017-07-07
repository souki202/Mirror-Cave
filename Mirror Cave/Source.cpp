#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <array>
#include <stack>
#include <cstring>

enum PassageType{
	FREE, WALL, SEARCHED, GOAL,
};

bool isSearched[50][50][50][50] = {false};

struct Point {
	Point() { x = 0; y = 0; };
	Point(char x, char y) {
		this->x = x;
		this->y = y;
	};
	char x, y;
};

int main() {
	int w, h;

	while (std::cin >> w >> h && w && h) {
		std::memset(isSearched, false, sizeof(bool) * 50 * 50 * 50 * 50);
		std::array<std::vector<std::vector<int>>, 2> room;
		Point start[2];
		for (int i = 0; i < h; i++) {
			std::array<std::string, 2> str;
			for (int j = 0; j < str.size(); j++) {
				std::cin >> str[j];
				room[j].push_back(std::vector<int>());
				for (int k = 0; k < str[j].size(); k++) {
					int type = 0;
					if (str[j][k] == '.') type = FREE;
					else if (str[j][k] == '#') type = WALL;
					else if (str[j][k] == '%') type = GOAL;
					else if (str[j][k] == 'L' || str[j][k] == 'R') {
						start[j].x = k;
						start[j].y = i;
						type = SEARCHED;
					}
					room[j].back().push_back(type);
				}
			}
		}

		std::stack<std::pair<Point, Point>> s;
		s.push(std::make_pair(start[0], start[1]));
		int dx[] = {1, 0, -1, 0};
		int dy[] = {0, 1, 0, -1};

		bool isOk = false;
		while (!s.empty() && !isOk) {
			auto node = s.top(); s.pop();

			//次に行ける場所を探す
			for (int i = 0; i < 4; i++) {
				auto next = node;
				next.first.x += dx[i];
				next.first.y += dy[i];
				next.second.x -= dx[i];
				next.second.y += dy[i];

				//場外補正
				next.first.x = std::min(std::max((char)0, next.first.x), (char)(w - 1));
				next.first.y = std::min(std::max((char)0, next.first.y), (char)(h - 1));
				next.second.x = std::min(std::max((char)0, next.second.x), (char)(w - 1));
				next.second.y = std::min(std::max((char)0, next.second.y), (char)(h - 1));
				//どちらもゴールならOK
				if (room[0][next.first.y][next.first.x] == GOAL && room[1][next.second.y][next.second.x] == GOAL) {
					isOk = true;
				}
				else {
					//壁があるなら元の位置へ
					if (room[0][next.first.y][next.first.x] == WALL) {
						next.first = node.first;
					}
					if (room[1][next.second.y][next.second.x] == WALL) {
						next.second = node.second;
					}
					//探索前ならスタックに追加。両方ゴールのパターンは事前に確認済み。
					if (!isSearched[next.first.y][next.first.x][next.second.y][next.second.x] && room[0][next.first.y][next.first.x] != GOAL && room[1][next.second.y][next.second.x] != GOAL) {
						s.push(next);
						isSearched[next.first.y][next.first.x][next.second.y][next.second.x] = true;
					}
				}
			}
		}

		std::cout << (isOk ? "Yes" : "No") << std::endl;
	}
	return 0;
}