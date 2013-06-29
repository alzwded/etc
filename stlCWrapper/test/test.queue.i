#define TEST_NAME "queue"
void testQueue()
{
    HANDLE queue;
    int b;
    char* str1;
    char* str2;
    char* retr;

    f = fopen("queue.log", "w");

    fprintf(f, "-----------------------------------------------------------------------\n");
    fprintf(f, "testing queues\n");
    fprintf(f, "-----------------------------------------------------------------------\n");

    str1 = (char*) malloc(2 * sizeof(char));
    str2 = (char*) malloc(2 * sizeof(char));
    strcpy(str1, "1");
    strcpy(str2, "2");
    fprintf(f, "str1 %s@%p\n", str1, str1);
    fprintf(f, "str2 %s@%p\n", str2, str2);
    fprintf(f, "-----------------------------------------------------------------------\n");

    queue = queue_create();
    assert(queue_isempty(queue, &b));
    assert(b != 0);
    assert(queue_pushd(queue, str1, &remove_a));
    assert(queue_push(queue, str2));
    assert(queue_isempty(queue, &b));
    assert(b == 0);

    queue_front(queue, (void**)(&retr));
    fprintf(f, "got  %s@%p\n", retr, retr);
    assert(retr == str1);
    assert((strlen(retr) == strlen(str1)) && (strcmp(retr, str1) == 0));

    queue_back(queue, (void**)(&retr));
    fprintf(f, "got  %s@%p\n", retr, retr);
    assert(retr == str2);
    assert((strlen(retr) == strlen(str2)) && (strcmp(retr, str2) == 0));

    assert(queue_pop(queue));
    queue_front(queue, (void**)(&retr));
    fprintf(f, "got  %s@%p\n", retr, retr);
    assert(retr == str2);
    assert((strlen(retr) == strlen(str2)) && (strcmp(retr, str2) == 0));

    assert(queue_isempty(queue, &b));
    assert(b == 0);

    queue_pop(queue);
    assert(queue_isempty(queue, &b));
    assert(b != 0);

    queue_delete(queue);
    assert(!queue_isempty(queue, &b));
    fprintf(f, "-----------------------------------------------------------------------\n");
    printf("queue passed\n");
    fclose(f);
}
#undef TEST_NAME
