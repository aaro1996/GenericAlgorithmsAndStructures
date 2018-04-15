#define TYPENAME int
#define GENTYPE int

//Less than
#define GENTYPE_LT(item1, item2) (item1 < item2)
//Less than or equal to
#define GENTYPE_LTE(item1, item2) (item1 <= item2)
//Greater than
#define GENTYPE_GT(item1, item2) (item1 > item2)
//Greater than or equal to
#define GENTYPE_GTE(item1, item2) (item1 >= item2)
//Equal to
#define GENTYPE_EQ(item1, item2) (item1 == item2)

#define GENTYPE_INTIFY(item1) (item1)

#define GENTYPE_PRINT_LENGTH 6
#define GENTYPE_STRINGIFY(item1, string_target, buffer_size) (snprintf(string_target, buffer_size, "%06d", item1))