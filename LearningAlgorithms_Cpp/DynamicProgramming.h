
#pragma once

#ifndef DP_H
#define DP_H

#include <algorithm>
#include <iostream>

namespace DynamicProgramming
{
	class triangle
	{
	private:
		static const int MAX = 100;
		static int D[MAX][MAX];
		static int n;
		static int DP[MAX][MAX];
	public:
		static int MaxSum_DG(int i, int j)
		{
			if (i == n - 1)
				return D[i][j];
			int x = MaxSum_DG(i + 1, j);
			int y = MaxSum_DG(i + 1, j + 1);
			return std::max(x, y) + D[i][j];
		}
		static int MaxSum_DP1(int i, int j)
		{
			if (i == n - 1)
				return (DP[i][j] = D[i][j]);
			if (DP[i][j] != -1)
				return DP[i][j];
			int x = MaxSum_DP1(i + 1, j);
			int y = MaxSum_DP1(i + 1, j + 1);
			DP[i][j] = std::max(x, y);
			return (DP[i][j] += D[i][j]);
		}
		static int MaxSum_DP2(int i, int j)
		{
			for (int jndex = 0; jndex < n; jndex++)
				DP[n - 1][jndex] = D[n - 1][jndex];
			for (int index = n - 1 - 1; index >= 0; index--)
			{
				for (int jndex = 0; jndex < index; jndex++)
				{
					DP[index][jndex] = D[index][jndex] + std::max(DP[index + 1][jndex], DP[index + 1][jndex + 1]);
				}
			}
			return DP[i - 1][j - 1];
		}
		static void input()
		{
			using std::cin;
			int i, j;
			auto cptr = cin.tie(nullptr);
			//cin >> n;
			n = 100;
			std::srand(0);
			for (i = 1; i <= n; i++)
				for (j = 1; j <= i; j++)
				{
					//cin >> D[i][j];
					do {
						D[i][j] = std::rand();
					} while (D[i][j] == -1);
					DP[i][j] = -1;
				}
			cin.tie(cptr);
		}
	};

	int triangle::D[MAX][MAX];
	int triangle::n;
	int triangle::DP[MAX][MAX];

	class sub_string
	{
	private:
		static const int MAX = 101;
		static int mss[MAX][MAX];
	public:
		static int max_length_of_sub_str(std::string S1, std::string S2)
		{
			#pragma warning(push)
			#pragma warning(disable:6385)
			const int N1 = S1.length();
			const int N2 = S2.length();
			if (N1 > MAX || N2 > MAX)return -1;
			if (N1 < 0 || N2 < 0)return -1;
			for (int i = 0; i <= N1; i++)
				mss[i][0] = 0;
			for (int j = 1; j <= N2; j++)
				mss[0][j] = 0;
			for (int i = 1; i <= N1; i++)
				for (int j = 1; j <= N2; j++)
					if (S1[i - 1] == S2[j - 1])
						mss[i][j] = 1 + mss[i - 1][j - 1];
					else
						mss[i][j] = std::max(mss[i][j - 1], mss[i - 1][j]);
			return mss[N1][N2];
			#pragma warning(pop)
		}
	};

	int sub_string::mss[MAX][MAX];
};

#endif // !DP_H