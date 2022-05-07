#ifndef ARCHH
#define ARCHH

#define NAME_OFFCET 0
#define NAME_LENGTH 100
#define MODE_OFFCET 100
#define MODE_LENGTH 8
#define UID_OFFCET 108
#define UID_LENGTH 8
#define GID_OFFCET 116
#define GID_LENGTH 8
#define SIZE_OFFCET 124
#define SIZE_LENGTH 12
#define MTIME_OFFCET 136
#define MTIME_LENGTH 12
#define CHKSUM_OFFCET 148
#define CHKSUM_LENGTH 8
#define TYPEFLAG_OFFCET 156
#define TYPEFLAG_LENGTH 1
#define LINKNAME_OFFCET 156
#define LINKNAME_LENGTH 100
#define MAGIC_OFFCET 257
#define MAGIC_LENGTH 6
#define VERSION_OFFCET 263
#define VERSION_LENGTH 2
#define UNAME_OFFCET 265
#define UNAME_LENGTH 32
#define GNAME_OFFCET 297
#define GNAME_LENGTH 32
#define DEVMAJOR_OFFCET 329
#define DEVMAJOR_LENGTH 8
#define DEVMINOR_OFFCET 337
#define DEVMINOR_LENGTH 8
#define PREFIX_OFFCET 345
#define PREFIX_LENGTH 155
#define BUFFER_LENGTH 12
#define BLOCK_SIZE 512
#define MASK 07777
#define FULL_PATH_LEN 255

typedef struct node{
  char *path;
  struct node *next;

} node;


typedef struct list{
  node *head;
  node *tail;

} list;

typedef struct __attribute__ ((__packed__)) header {

  char name[NAME_LENGTH];
  char mode[MODE_LENGTH];
  char uid[UID_LENGTH];
  char gid[GID_LENGTH];
  char size[SIZE_LENGTH];
  char mtime[MTIME_LENGTH];
  char chksum[CHKSUM_LENGTH];
  char typeflag[TYPEFLAG_LENGTH];
  char linkname[LINKNAME_LENGTH];
  char magic[MAGIC_LENGTH];
  char version[VERSION_LENGTH];
  char uname[UNAME_LENGTH];
  char gname[GNAME_LENGTH];
  char devmajor[DEVMAJOR_LENGTH];
  char decminor[DEVMINOR_LENGTH];
  char prefix[PREFIX_LENGTH];
  char pad[BUFFER_LENGTH];

} header;

header *make_header(struct stat toTar, char *path, int S);
int archive_creator(int infile, int outfile, char *path,int S, int v);
int archive_get(int tarfile, int v);
int pad_ending(int tarfile);
int print_table(int tarfile, int v);
int print_table_name(int tarfile, char *name, int v);

#endif
