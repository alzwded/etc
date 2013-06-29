#define TEST_NAME "stringMap"
void testStringMap()
{
    char* str1;
    char* str2;
    char* str3;
    char* retreived;
    HANDLE oneMap = stringmap_create();
    int hasIt;

    f = fopen("stringmap.log", "w");

    fprintf(f, "-----------------------------------------------------------------------\n");
    fprintf(f, "testing string stringmaps\n");
    fprintf(f, "-----------------------------------------------------------------------\n");

    str1 = (char*) malloc(4);
    strcpy(str1, "123");
    str2 = (char*) malloc(4);
    strcpy(str2, "321");
    str3 = (char*) malloc(4);
    strcpy(str3, "696");

    fprintf(f, "str1@%p\nstr2@%p\nstr3@%p\n", str1, str2, str3);
    fprintf(f, "-----------------------------------------------------------------------\n");

    fprintf(f, "insert with string key and dealloc... ");
    assert(stringmap_setd(oneMap, "key", str1, remove_a));
    fprintf(f, "OK\n");

    fprintf(f, "item was successfully added... ");
    stringmap_haskey(oneMap, "key", &hasIt);
    assert(hasIt == 1);
    fprintf(f, "OK\n");

    fprintf(f, "access... ");
    assert(stringmap_get(oneMap, "key", (void**)(&retreived)));
    assert(retreived == str1);
    assert((strlen(retreived) == strlen(str1)) && (strcmp(retreived, str1) == 0));
    fprintf(f, "OK\n");

    fprintf(f, "replacing item with key... ");
    expect_remove();
    assert(stringmap_setd(oneMap, "key", str2, remove_a));
    assert(stringmap_get(oneMap, "key", (void**)(&retreived)));
    assert(retreived == str2);
    assert((strlen(retreived) == strlen(str2)) && (strcmp(retreived, str2) == 0));
    fprintf(f, "OK\n");
    fprintf(f, "dealloc was called on previous item... ");
    assert(remove_called);
    fprintf(f, "OK\n");

    fprintf(f, "insert with defaul dealloc... ");
    assert(stringmap_set(oneMap, "KEY", str3));
    fprintf(f, "OK\n");

    expect_remove();
    fprintf(f, "removing item with string key... ");
    assert(stringmap_remove(oneMap, "key"));
    stringmap_haskey(oneMap, "key", &hasIt);
    assert(hasIt == 0);
    fprintf(f, "OK\n");
    fprintf(f, "remove_a was called... ");
    assert(remove_called);
    fprintf(f, "OK\n");

    fprintf(f, "fail code when trying to access nonexistant element... ");
    assert(0 == stringmap_get(oneMap, "key", (void**)(&retreived)));
    fprintf(f, "OK\n");

    fprintf(f, "testing other key... ");
    assert(stringmap_get(oneMap, "KEY", (void**)(&retreived)));
    assert(retreived == str3);
    assert((strlen(str3) == strlen(retreived)) && (strcmp(str3, retreived) == 0));
    fprintf(f, "OK\n");

    fprintf(f, "removing map... ");
    stringmap_delete(oneMap);
    assert(!stringmap_set(oneMap, "key", str1));
    fprintf(f, "OK\n");

    fprintf(f, "testing null passing... ");
    oneMap = stringmap_create();
    stringmap_setd(oneMap, "", "lalala", NO_DEALLOC);
    stringmap_get(oneMap, "", (void**)&retreived);
    assert(strcmp(retreived, "lalala") == 0);
    fprintf(f, "OK\n");

    fprintf(f, "no dealloc doesn't crash... ");
    stringmap_delete(oneMap);
    fprintf(f, "OK\n");

    fprintf(f, "testing error handling... ");
    oneMap = stringmap_create();
    stringmap_setd(oneMap, "a", "lalala", NO_DEALLOC);
    assert(0 == stringmap_get(oneMap, 0, (void**)&retreived));
    fprintf(f, "OK\n");
    fprintf(f, "-----------------------------------------------------------------------\n");
    printf("stringMap passed\n");
    fclose(f);
}
#undef TEST_NAME
