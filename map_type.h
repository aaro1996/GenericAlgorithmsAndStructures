#define TYPENAME map
#define GENTYPE struct map *

//Less than
#define GENTYPE_LT(item1, item2) (item1->key < item2->key)
//Less than or equal to
#define GENTYPE_LTE(item1, item2) (item1->key <= item2->key)
//Greater than
#define GENTYPE_GT(item1, item2) (item1->key > item2->key)
//Greater than or equal to
#define GENTYPE_GTE(item1, item2) (item1->key >= item2->key)
//Equal to
#define GENTYPE_EQ(item1, item2) (item1->key == item2->key)

#define GENTYPE_INTIFY(item1) (item1->key)

#define GENTYPE_PRINT_LENGTH 13
#define GENTYPE_STRINGIFY(item1, string_target, buffer_size) (snprintf(string_target, buffer_size, "k:%04d v:%04d", item1->key, item1->val))

struct map {
	int key;
	int val;
};