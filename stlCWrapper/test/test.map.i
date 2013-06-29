#define TEST_NAME "map"
void testMap()
{
    char* str1;
    char* str2;
    char* str3;
    char* retreived;
    HANDLE oneMap = map_create();
    int hasIt;

    f = fopen("map.log", "w");

    fprintf(f, "-----------------------------------------------------------------------\n");
    fprintf(f, "testing maps\n");
    fprintf(f, "-----------------------------------------------------------------------\n");

    str1 = (char*) malloc(4);
    strcpy(str1, "123");
    str2 = (char*) malloc(4);
    strcpy(str2, "321");
    str3 = (char*) malloc(4);
    strcpy(str3, "696");

    fprintf(f, "str1@%p\nstr2@%p\nstr3@%p\n", str1, str2, str3);
    fprintf(f, "-----------------------------------------------------------------------\n");

    map_setd(oneMap, 0, str1, remove_a);
    fprintf(f, "after insert\n");
    map_get(oneMap, 0, (void**)(&retreived));
    fprintf(f, "after access\n");
    fprintf(f, "printing: %s@%p\n", retreived, &retreived);
    fprintf(f, "should get: %s@%p\n", str1, str1);
    fprintf(f, "should not: %s@%p\n", str2, str2);
    assert(retreived == str1);
    assert((strlen(retreived) == strlen(str1)) && (strcmp(retreived, str1) == 0));

    expect_remove();
    map_setd(oneMap, 0, str2, remove_a);
    assert(remove_called);
    fprintf(f, "after insert\n");
    map_set(oneMap, 1, str3);
    fprintf(f, "after default insert\n");

    map_get(oneMap, 0, (void**)(&retreived));
    fprintf(f, "after access\n");
    fprintf(f, "printing: %s@%p\n", retreived, &retreived);
    fprintf(f, "should get: %s@%p\n", str2, str2);
    fprintf(f, "should not: %s@%p\n", str1, str1);
    assert(retreived == str2);
    assert((strlen(retreived) == strlen(str2)) && (strcmp(retreived, str2) == 0));
    map_remove(oneMap, 0);
    fprintf(f, "after remove\n");

    map_get(oneMap, 1, (void**)(&retreived));
    fprintf(f, "after access\n");
    fprintf(f, "printing: %s@%p\n", retreived, &retreived);
    fprintf(f, "should get: %s@%p\n", str3, str3);
    assert(retreived == str3);
    assert((strlen(str3) == strlen(retreived)) && (strcmp(str3, retreived) == 0));

    map_haskey(oneMap, 0, &hasIt);
    fprintf(f, "should be 0: %d\n", hasIt);
    assert(hasIt == 0);
    fprintf(f, "should be 0: %d\n", map_get(oneMap, 0, (void**)(&retreived)));
    assert(0 == map_get(oneMap, 0, (void**)(&retreived)));

    map_delete(oneMap);
    assert(!map_set(oneMap, 0, str1));

    fprintf(f, "-----------------------------------------------------------------------\n");
    printf("map passed\n");
    fclose(f);
}
#undef TEST_NAME
