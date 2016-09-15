---
title: 动态规划:最长公共子序列
categories: 动态规划
tags: [算法,动态规划] 

---
<script type="text/javascript"
   src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>

**转载请注明出处**：[blog.justkitt.com](http://blog.justkitt.com)

## 问题1
有两个序列，分别是s1 = {a,b,c,b,d,a,b},s2 = {b,d,c,a,b,a}，从前往后找，s1和s2的最长公共子序列为LCS(s1,s2) = {b,c,b,a}，求s1和s2的最长公共子序列长度。
<!-- more -->
## 问题解答
### 思路
LCS问题的最优解只取决于其子序列LCS问题的最优解，非最优解对于问题的求解没有影响。同时，子序列LCS的值和当前对比字符的值一旦确定，则此后过程的LCS计算不再受此前各状态的影响。满足[动态规划](https://zh.wikipedia.org/zh/%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92)的条件。动态规划的解题思路主要就是求出状态转移方程，然后根据状态转移方程自底向上得到最终的结果（一般用一个数组保存状态，用循环进行状态变更）
### 状态转移方程
因为有两个序列，用一个二维数组dp[s1.length+1][s2.length+1]来表示。dp[i][j]表示s1[1-i]子串和s2[1-j]子串的最长公共子序列的长度，其中s[1-i]表示的是长度为i的s的子串。那么我们可以得到如下公式：
$$
        dp[i][j] =
        \\begin{cases}
        max(dp[i-1][j],dp[i][j-1]),  & \\text{if s1[i-1] == s2[j-1]} \\\\
        dp[i-1][j-1]+1, & \\text{other}
        \\end{cases}
$$
初始条件是\\( dp[0][0] = 0 \\)，返回值是\\( dp[s1.length][s2.length]\\)

### 代码
有了如上的分析，很容易得到代码，我们只需要用一个循环逐渐完成整个操作就可以了。
```C++
int LCS(string& s1,string& s2){
	vector<vector<int>> dp(s1.size()+1,vector<int>(s2.size()+1,0));
	for(int i = 1;i<=s1.size();++i){
		for(int j = 1;j<=s2.size();++j){
			if(s1[i-1] == s2[j-1])
				dp[i][j] = dp[i-1][j-1]+1;
			else
				dp[i][j] = max(dp[i-1][j],dp[i][j-1]);
		}
	}
	return dp[s1.size()][s2.size()];
}
```
## 问题2
有两个序列，分别是s1和s2，从前往后找，s1和s2的最长公共子序列是什么？

## 问题解答
上面的问题解答仅仅将最长子序列的长度输出了，但是没有得到最长的子序列是什么，接下来在知道了dp数组之后，可以求得最长子序列。代码如下：

```C++
string LCS(string& s1, string& s2){
	vector<vector<int>> dp(s1.size()+1,vector<int>(s2.size()+1,0));
	for(int i = 1;i<=s1.size();++i){
		for(int j = 1;j<=s2.size();++j){
			if(s1[i-1] == s2[j-1])
				dp[i][j] = dp[i-1][j-1]+1;
			else
				dp[i][j] = max(dp[i-1][j],dp[i][j-1]);
		}
	}
	string ret;
	int s1pos = s1.size();
	int s2pos = s2.size();
	while(s1pos!=0&&s2pos!=0){
		if(s1[s1pos-1] == s2[s2pos-1]){
			ret.push_back(s1[s1pos-1]);
			--s1pos;
			--s2pos;
		}
		else if(dp[s1pos][s2pos-1] >= dp[s1pos-1][s2pos])
			--s2pos;
		else
			--s1pos;
	}
	reverse(ret.begin(),ret.end());
	return ret;
}
```
以上代码只是求出其中一种LCS而已，LCS会有很多种情况，所以一般仅仅会问最长子序列的长度是多少。