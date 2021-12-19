#include <iostream>
#include <queue>
using namespace std;
struct TreeNode
{
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode()
		:val(0)
		, left(nullptr)
		, right(nullptr)
	{

	}
};
class Mytree
{
public:
	Mytree()
		:root(nullptr)
	{

	}
	TreeNode* init(vector<int>& nums, int n = 0)
	{
		if (n >= nums.size())
			return nullptr;
		TreeNode* tmp = new TreeNode();
		tmp->val = nums[n];
		tmp->left = init(nums, n * 2 + 1);
		tmp->right = init(nums, n * 2 + 2);
		root = tmp;
		return root;
	}
	int maxDepth()
	{
		if (root == nullptr)
			return 0;
		queue<TreeNode*> q;
		q.push(root);
		int ret = 0;
		while (!q.empty())
		{
			int size = q.size();
			ret++;
			for (int i = 0; i < size; i++)
			{
				TreeNode* cur = q.front();
				q.pop();
				if (cur->left)
					q.push(cur->left);
				if (cur->right)
					q.push(cur->right);
			}
		}
		return ret;
	}
private:
	TreeNode* root;
};
void test()
{
	Mytree mt;
	vector<int> v = { 3,9,20,15,7 };
	mt.init(v);
	cout << mt.maxDepth() << endl;

}

int main()
{
	test();
	return 0;
}