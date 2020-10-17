#define F first
#define S second
#define PB push_back
#define MP make_pair

// v.push_back(make_pair(y1,x1));
// v.push_back(make_pair(y2,x2)); 
// int d = v[i].first + v[i].second;

// shorten this code using macros

v.PB(MP(y1,x1));
v.PB(MP(y2,x2));
int d = v[i].F + v[i].S;