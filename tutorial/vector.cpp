vector<int> v;
v.push_back(3); 
v.push_back(2); 
v.push_back(5); 

//elements can be accessed like a normal array
cout << v[0] << "\n"; // 3
cout << v[1] << "\n"; // 2
cout << v[2] << "\n"; // 5

//use the function size to return the number of elements in a vector
for (int i = 0; i < v.size(); i++) { 
    cout << v[i] << "\n";
}
//shorthand for iterating
for (auto x : v) {
       cout << x << "\n";
}

cout << v.back() << "\n"; // 5 //shows the last element
v.pop_back(); //removes the last element
cout << v.back() << "\n"; // 2