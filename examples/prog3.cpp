int func(){
    return 3;
}

int main(){
    int* x = new int;

    x[0] = func();

    if (x[0] == 1) {
        return 0;
    }
    else if (x[0] == 3) {
        return 5;
    }

    return 0;
}