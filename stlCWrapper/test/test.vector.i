#define TEST_NAME "vector"
void testVector()
{
    HANDLE vector = NULL_HANDLE;
    int b;
    int i;
    int x = 1;
    int y = 2;
    int z = 3;
    int* retr = NULL;
    char* str1 = NULL;
    char* str2 = NULL;
    char* str3 = NULL;
    char* str4 = NULL;
    char* str5 = NULL;
    char* pretr = NULL;
    str1 = (char*) malloc(4 * sizeof(char));
    str2 = (char*) malloc(4 * sizeof(char));
    str4 = (char*) malloc(4 * sizeof(char));
    str5 = (char*) malloc(4 * sizeof(char));
    str3 = (char*) malloc(2 * sizeof(char));
    strcpy(str1, "asd");
    strcpy(str2, "dsa");
    strcpy(str3, "x");
    strcpy(str4, "111");
    strcpy(str5, "222");

    f = fopen("vector.log", "w");

    fprintf(f, "-----------------------------------------------------------------------\n");
    fprintf(f, "testing vectors\n");
    fprintf(f, "-----------------------------------------------------------------------\n");
    fprintf(f, "%s@%p\n%s@%p\n%s@%p\n", str1, str1, str2, str2, str3, str3);
    fprintf(f, "-----------------------------------------------------------------------\n");
    fflush(f);

    fprintf(f, "Creating vector... ");
    vector = vector_create();
    assert(vector != NULL_HANDLE);
    fprintf(f, "OK\n");

    fprintf(f, "Pushing back 3 ints... ");
    assert(vector_push_backd(vector, (void*)&x, NO_DEALLOC));
    assert(vector_push_backd(vector, (void*)&y, NO_DEALLOC));
    assert(vector_push_backd(vector, (void*)&z, NO_DEALLOC));
    fprintf(f, "OK\n");

    fprintf(f, "Size... ");
    assert(vector_size(vector, &i));
    assert(i == 3);
    fprintf(f, "OK\n");

    fprintf(f, "Empty should return false... ");
    assert(vector_empty(vector, &b));
    assert(b == 0);
    fprintf(f, "OK\n");

    fprintf(f, "Getting 2nd element... ");
    assert(vector_get(vector, 1, (void**)&retr));
    assert(*retr == 2);
    fprintf(f, "OK\n");

    fprintf(f, "Removing first two element... ");
    assert(vector_erase_range(vector, 0, 2));
    fprintf(f, "OK\n");

    fprintf(f, "Front... ");
    assert(vector_front(vector, (void**)&retr));
    assert(*retr == 3);
    fprintf(f, "OK\n");

    fprintf(f, "Back... ");
    assert(vector_back(vector, (void**)&retr));
    assert(*retr == 3);
    fprintf(f, "OK\n");

    fprintf(f, "Pop last element... ");
    assert(vector_pop_back(vector));
    fprintf(f, "OK\n");

    fprintf(f, "Empty should return true... ");
    assert(vector_empty(vector, &b));
    assert(b == 1);
    fprintf(f, "OK\n");

    vector_delete(vector);

    fprintf(f, "Create with reserve space... ");
    assert((vector = vector_create_reserve(2)) != NULL_HANDLE);
    fprintf(f, "OK\n");

    fprintf(f, "Assigning two strings... ");
    assert(vector_setd(vector, 0, (void*)str1, remove_a));
    assert(vector_set(vector, 1, (void*)str2));
    fprintf(f, "OK\n");
    fflush(f);

    fprintf(f, "Get 2nd element... ");
    vector_get(vector, 1, (void**)&pretr);
    assert(pretr == str2);
    assert(strcmp(pretr, str2) == 0);
    fprintf(f, "OK\n");
    fflush(f);

    fprintf(f, "Insert 2 strings... ");
    assert(vector_insert_ranged(vector, 1, 2, (void*)str3, NO_DEALLOC));
    fprintf(f, "OK\n");
    fflush(f);

    fprintf(f, "Get 2nd element... ");
    fflush(f);
    vector_get(vector, 1, (void**)&pretr);
    assert(pretr == str3);
    assert(strcmp(pretr, str3) == 0);
    fprintf(f, "OK\n");
    fflush(f);

    fprintf(f, "Size is 4... ");
    vector_size(vector, &i);
    assert(i == 4);
    fprintf(f, "OK\n");
    fflush(f);

    fprintf(f, "Remove one item... ");
    assert(vector_erase(vector, 1));
    fprintf(f, "OK\n");
    fflush(f);

    fprintf(f, "Get 3rd element... ");
    vector_get(vector, 2, (void**)&pretr);
    assert(pretr == str2);
    assert(strcmp(pretr, str2) == 0);
    fprintf(f, "OK\n");
    fflush(f);

    vector_delete(vector);

    // this corrupts memory, wtf?
    fprintf(f, "testing that destructor gets called... ");
    fflush(f);
    vector = vector_create();
    vector_push_backd(vector, str4, remove_a);
    vector_push_backd(vector, str5, remove_a);
    expect_remove();
    vector_pop_back(vector);
    assert(remove_called);
    expect_remove();
    vector_pop_back(vector);
    assert(remove_called);
    vector_empty(vector, &b);
    assert(b);
    fprintf(f, "OK\n");
    fflush(f);

    fprintf(f, "deleting vector... ");
    fflush(f);
    vector_delete(vector);
    fprintf(f, "OK\n");
    fflush(f);

    fprintf(f, "testing reserve space reserves enough space... ");
    fflush(f);
    assert((vector = vector_create_reserve(200)) != NULL_HANDLE);
    assert(vector_size(vector, &i));
    assert(i == 200);
    fprintf(f, "OK\n");
    fflush(f);

    fprintf(f, "deleting vector... ");
    vector_delete(vector);
    fprintf(f, "OK\n");

    fprintf(f, "-----------------------------------------------------------------------\n");
    printf("vector passed\n");
    fclose(f);
}
#undef TEST_NAME
