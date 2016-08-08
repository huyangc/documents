---
title: 动态规划:背包问题
categories: 动态规划
tags: [算法,动态规划] 

---
<script type="text/javascript"
   src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>

**转载请注明出处**：[blog.justkitt.com](http://blog.justkitt.com)

# 问题定义
背包问题可以被描述为决定性问题，即在总重量不超过\\(W\\)的前提下，总价值是否能达到\\(V\\)。
共有\\(n\\)种物品，物品\\(i\\)的重量是\\(w_i\\)，价格是\\(v_i\\)。假设所有物品的重量和价格都是非负的，背包所能承受的最大重量为\\(W\\)。
具体根据物品可以选取的次数，又可以将问题分为以下三类
## 0-1背包问题
即每个物品只能选取0个或者1个，可以将问题用下述公式表示
最大化\\(\\sum_{i=1}^n p_ix_i\\)

受限于\\(\\sum_{i=1}^n w_ix_i \\leq W ,  x_i \\in [0,1]\\)

## 有界背包问题
限定物品\\(i\\)最多只能有\\(b_i\\)个，则问题成为有界背包问题。可以用下述公式表达：

最大化 \\(\\sum_{i=1}^n p_ix_i\\)

受限于\\(\\sum_{i=1}^n w_ix_i \\leq W ,  x_i \\in [0,b_j]\\)

## 无界背包问题
不限定每种物品的数量，则称为无界背包问题。

# 问题转换
各类背包问题都可以转换为简单的0-1背包问题来解决。
## 0-1背包问题解决方法
我们假设第i件物品的重量是w[i]，价值是p[i]，背包总共能够容纳的容量是W。
那么我们设置状态转移变量为dp[i][w]表示的是前i件物品恰好是w那么重能够获得的最大的价值。
所以状态转移方程为：
$$
dp[i][w] = max(dp[i-1][w],dp[i-1][w-w[i]]+p[i])
$$
这个状态转移方程的意义可以有两个，首先我们判断的是第i件物品到底放不放，如果第i件不放入背包，那么当前就是前i-1件商品在w重量的情况下的价值，如果第i件商品放入背包，那么就是前i-1件商品放入背包并且要空出w[i]这么多重量给当前物品，然后用之前的状态加上p[i]就是放入第i件物品之后背包里的价值。
## 代码
经过以上的分析之后，可以得出以下的0-1背包问题的最不优化的情况的代码
```C++
int maxValue(vector<int>& w,int maxweight, vector<int>& p){
	vector<vector<int>> dp(w.size()+1,vector<int>(maxweight+1,0));
	for(int i = 1;i<=w.size();++i){
		for(int j = 0;j<=maxweight;++j){
			if(j<w[i-1])
				dp[i][j] = dp[i-1][j];
			else
				dp[i][j] = max(dp[i-1][j],dp[i-1][j-w[i-1]]+p[i-1]);
		}
	}
	return dp[w.size()][maxweight];
}
```