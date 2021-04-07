#ifndef TYPELIST_H
#define TYPELIST_H

/* Return the offset of 'member' relative to the beginning of a struct type */
#define offsetof(type, member)                                      \
((size_t)(&((type *)0)->member))

/* *
 * to_struct - get the struct from a ptr
 * @ptr:    a struct pointer of member
 * @type:   the type of the struct this is embedded in
 * @member: the name of the member within the struct
 * */
#define to_struct(ptr, type, member)                               \
((type *)((char *)(ptr) - offsetof(type, member)))


struct list_entry_t {
    struct list_entry_t *prev, *next;
};
static inline void list_init(struct list_entry_t *elm) __attribute__((always_inline));
static inline void list_add(struct list_entry_t *listelm, struct list_entry_t *elm) __attribute__((always_inline));
static inline void list_add_before(struct list_entry_t *listelm, struct list_entry_t *elm) __attribute__((always_inline));
static inline void list_add_after(struct list_entry_t *listelm, struct list_entry_t *elm) __attribute__((always_inline));
static inline void list_del(struct list_entry_t *listelm) __attribute__((always_inline));
static inline void list_del_init(struct list_entry_t *listelm) __attribute__((always_inline));
static inline bool list_empty(struct list_entry_t *list) __attribute__((always_inline));
static inline struct list_entry_t *list_next(struct list_entry_t *listelm) __attribute__((always_inline));
static inline struct list_entry_t *list_prev(struct list_entry_t *listelm) __attribute__((always_inline));

static inline void __list_add(struct list_entry_t *elm, struct list_entry_t *prev, struct list_entry_t *next) __attribute__((always_inline));
static inline void __list_del(struct list_entry_t *prev, struct list_entry_t *next) __attribute__((always_inline));
/* *
 * list_init - initialize a new entry
 * @elm:        new entry to be initialized
 * */
static inline void list_init(struct list_entry_t* elm) {
    elm->prev = elm->next = elm;
}

/* *
 * list_add - add a new entry
 * @listelm:    list head to add after
 * @elm:        new entry to be added
 *
 * Insert the new element @elm *after* the element @listelm which
 * is already in the list.
 * */
static inline void list_add(struct list_entry_t* listelm, struct list_entry_t* elm) {
    __list_add(elm, listelm, listelm->next);
}

/* *
 * list_add_before - add a new entry
 * @listelm:    list head to add before
 * @elm:        new entry to be added
 *
 * Insert the new element @elm *before* the element @listelm which
 * is already in the list.
 * */
static inline void list_add_before(struct list_entry_t* listelm, struct list_entry_t* elm) {
    __list_add(elm, listelm->prev, listelm);
}

/* *
 * list_add_after - add a new entry
 * @listelm:    list head to add after
 * @elm:        new entry to be added
 *
 * Insert the new element @elm *after* the element @listelm which
 * is already in the list.
 * */
static inline void list_add_after(struct list_entry_t* listelm, struct list_entry_t* elm) {
    __list_add(elm, listelm, listelm->next);
}

/* *
 * list_del - deletes entry from list
 * @listelm:    the element to delete from the list
 *
 * Note: list_empty() on @listelm does not return true after this, the entry is
 * in an undefined state.
 * */
static inline void list_del(struct list_entry_t* listelm) {
    __list_del(listelm->prev, listelm->next);
}

/* *
 * list_del_init - deletes entry from list and reinitialize it.
 * @listelm:    the element to delete from the list.
 *
 * Note: list_empty() on @listelm returns true after this.
 * */
static inline void list_del_init(struct list_entry_t* listelm) {
    list_del(listelm);
    list_init(listelm);
}

/* *
 * list_empty - tests whether a list is empty
 * @list:       the list to test.
 * */
static inline bool list_empty(struct list_entry_t* list) {
    return list->next == list;
}

/* *
 * list_next - get the next entry
 * @listelm:    the list head
 **/
static inline struct list_entry_t* list_next(struct list_entry_t* listelm) {
    return listelm->next;
}

/* *
 * list_prev - get the previous entry
 * @listelm:    the list head
 **/
static inline struct list_entry_t* list_prev(struct list_entry_t* listelm) {
    return listelm->prev;
}

/* *
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 * */
static inline void __list_add(struct list_entry_t* elm, struct list_entry_t* prev, struct list_entry_t* next) {
    prev->next = next->prev = elm;
    elm->next = next;
    elm->prev = prev;
}


/* *
 * Delete a list entry by making the prev/next entries point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 * */
static inline void __list_del(struct list_entry_t* prev, struct list_entry_t* next) {
    prev->next = next;
    next->prev = prev;
}

#endif // TYPELIST_H
