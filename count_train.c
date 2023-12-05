Train* scan_num_Trains(const char *fich, int *nbtrains) {
    FILE *file = fopen(fich, "r");
    if (file == NULL) {
   
        exit(1);
    }

    *nbtrains = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            (*nbtrains)++;
        }
    }
    Train *trains = (Train*)malloc(*nbtrains * sizeof(Train));
    return trains;
    fclose(fich);
}
