#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("--- Starting countpp Test ---\n");

  // 1. Get initial count
  uint64 start_pages = countpp();
  printf("Initial physical pages: %ld\n", start_pages);

  // 2. Request 10 pages of memory (Lazy)
  printf("Requesting 10 pages via sbrk(40960)...\n");
  char *p = sbrk(10 * 4096);
  if(p == (char*)-1){
    printf("sbrk failed\n");
    exit(1);
  }

  // 3. Check count again
  // With Lazy Allocation, this count should be the SAME as start_pages
  uint64 mid_pages = countpp();
  printf("Physical pages after lazy sbrk: %ld\n", mid_pages);

  if(mid_pages == start_pages){
    printf("SUCCESS: Lazy allocation confirmed (Page count didn't rise).\n");
  } else {
    printf("ALERTT: Pages were allocated immediately (Not lazy!).\n");
  }

  // 4. "Touch" the pages to trigger the Page Faults
  printf("Touching 3 pages to trigger allocation...\n");
  p[0] = 'a';           // Touches Page 0
  p[4096] = 'b';        // Touches Page 1
  p[8192] = 'c';        // Touches Page 2

  // 5. Final count
  // This should be start_pages + 3
  uint64 end_pages = countpp();
  printf("Physical pages after touching 3 pages: %ld\n", end_pages);

  if(end_pages == start_pages + 3){
    printf("SUCCESS: Demand paging working as expected!\n");
  } else {
    printf("FAILURE: Page count is %ld, expected %ld\n", end_pages, start_pages + 3);
  }

  exit(0);
}
