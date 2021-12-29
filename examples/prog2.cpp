int main(){
    int* x = new int;

    x[0] = 0;

    if (x[0] == 1) {
        return 0;
    }
    else if (x[0] == 0) {
        return 5;
    }

    return 0;
}