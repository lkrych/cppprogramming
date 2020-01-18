map<string,int> m;
m["monkey"] = 4;
m["banana"] = 3;
m["harpsichord"] = 9;
cout << m["banana"] << "\n"; // 3

// if the value of a key is requested but the map does not contain it
// the key is automatically added to the map with a default value
cout << m["aybabtu"] << "\n"; // 0

//the function count checks if a key exists in a map

if (m.count("aybabtu")) {
    //key exists
}