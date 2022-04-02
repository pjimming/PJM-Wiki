class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> num;
        for (int i = 0; i < tokens.size(); i++) {
            string t = tokens[i];
            if (t == "+" || t == "-" || t == "*" || t == "/") {
                int a = num.top(); num.pop();
                int b = num.top(); num.pop();

                int x = 0;
                if (t == "+") x = a + b;
                if (t == "-") x = b - a;
                if (t == "*") x = a * b;
                if (t == "/") x = b / a;

                num.push(x);
            } else {
                num.push(atoi(t.c_str()));
            }
        }
        return num.top();
    }
};