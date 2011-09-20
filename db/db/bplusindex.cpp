#include "bplusindex.h"

#include "bson.h"

#include <string.h>
#include <boost/crc.hpp>

BPlusIndex::BPlusIndex()
{
    _head = NULL;
}

BPlusIndex::~BPlusIndex()
{
    //dtor
}

void BPlusIndex::add(BSONObj* elem, long filePos) {
    Index* index = new Index();
    index->key = elem;
    index->pos = filePos;

    insertElement(index);
}

Index* BPlusIndex::find(BSONObj* elem) {
    char* key = elem->toChar();
    boost::crc_32_type crc32;
    crc32.process_bytes(key, strlen(key));
    long value = crc32.checksum();

    IndexPointer* pointer = findNode(_head, value);
    if (pointer->elem == NULL) {
        Index* index = new Index();
        index->key = elem;
        pointer->elem = index;
    }
    return pointer->elem;
}

void BPlusIndex::remove(BSONObj* elem) {
}

IndexPointer* BPlusIndex::findNode(IndexPointer* start, long value) {
    if (_head == NULL) {
        _head = new IndexPointer;
        _head->left = NULL;
        _head->right = NULL;
        _head->value = value;
        _head->elem = NULL;
        return _head;
    } else {
        IndexPointer* current = start;
        if (current->value > value) {
            if (current->left == NULL) {
                IndexPointer* pointer = new IndexPointer();
                pointer->right = NULL;
                pointer->elem = NULL;
                pointer->value = value;
                current->left = pointer;
                return pointer;
            } else {
                return findNode(current->left, value);
            }
        } else if(current->value < value) {
            if (current->right == NULL) {
                IndexPointer* pointer = new IndexPointer();
                pointer->left = NULL;
                pointer->elem = NULL;
                pointer->value = value;
                current->right = pointer;
                return pointer;
            } else {
                return findNode(current->right, value);
            }
        } else {
            return current;
        }
    }
}

void BPlusIndex::insertElement(Index* elem) {
    BSONObj* obj = elem->key;
    char* key = obj->toChar();
    boost::crc_32_type crc32;
    crc32.process_bytes(key, strlen(key));
    long value = crc32.checksum();

    IndexPointer* node = NULL;
    if (!_head) {
        _head = new IndexPointer;
        _head->elem = elem;
        _head->left = 0;
        _head->right = 0;
        _head->value = value;
        node = _head;
    } else {
        IndexPointer* node = findNode(_head, value);
        node->elem = elem;
    }
}