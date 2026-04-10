//
// Generated file, do not edit! Created by opp_msgtool 6.2 from src/messages.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "messages_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(EnhancedNetMsg)

EnhancedNetMsg::EnhancedNetMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

EnhancedNetMsg::EnhancedNetMsg(const EnhancedNetMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

EnhancedNetMsg::~EnhancedNetMsg()
{
}

EnhancedNetMsg& EnhancedNetMsg::operator=(const EnhancedNetMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void EnhancedNetMsg::copy(const EnhancedNetMsg& other)
{
    this->messageId = other.messageId;
    this->sourceNode = other.sourceNode;
    this->destNode = other.destNode;
    this->sequenceNum = other.sequenceNum;
    this->creationTime = other.creationTime;
    this->ttl = other.ttl;
    this->hopCount = other.hopCount;
    this->retransmission = other.retransmission;
    this->payload = other.payload;
}

void EnhancedNetMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->messageId);
    doParsimPacking(b,this->sourceNode);
    doParsimPacking(b,this->destNode);
    doParsimPacking(b,this->sequenceNum);
    doParsimPacking(b,this->creationTime);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->retransmission);
    doParsimPacking(b,this->payload);
}

void EnhancedNetMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->messageId);
    doParsimUnpacking(b,this->sourceNode);
    doParsimUnpacking(b,this->destNode);
    doParsimUnpacking(b,this->sequenceNum);
    doParsimUnpacking(b,this->creationTime);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->retransmission);
    doParsimUnpacking(b,this->payload);
}

int EnhancedNetMsg::getMessageId() const
{
    return this->messageId;
}

void EnhancedNetMsg::setMessageId(int messageId)
{
    this->messageId = messageId;
}

int EnhancedNetMsg::getSourceNode() const
{
    return this->sourceNode;
}

void EnhancedNetMsg::setSourceNode(int sourceNode)
{
    this->sourceNode = sourceNode;
}

int EnhancedNetMsg::getDestNode() const
{
    return this->destNode;
}

void EnhancedNetMsg::setDestNode(int destNode)
{
    this->destNode = destNode;
}

int EnhancedNetMsg::getSequenceNum() const
{
    return this->sequenceNum;
}

void EnhancedNetMsg::setSequenceNum(int sequenceNum)
{
    this->sequenceNum = sequenceNum;
}

omnetpp::simtime_t EnhancedNetMsg::getCreationTime() const
{
    return this->creationTime;
}

void EnhancedNetMsg::setCreationTime(omnetpp::simtime_t creationTime)
{
    this->creationTime = creationTime;
}

int EnhancedNetMsg::getTtl() const
{
    return this->ttl;
}

void EnhancedNetMsg::setTtl(int ttl)
{
    this->ttl = ttl;
}

int EnhancedNetMsg::getHopCount() const
{
    return this->hopCount;
}

void EnhancedNetMsg::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

bool EnhancedNetMsg::getRetransmission() const
{
    return this->retransmission;
}

void EnhancedNetMsg::setRetransmission(bool retransmission)
{
    this->retransmission = retransmission;
}

const char * EnhancedNetMsg::getPayload() const
{
    return this->payload.c_str();
}

void EnhancedNetMsg::setPayload(const char * payload)
{
    this->payload = payload;
}

class EnhancedNetMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_messageId,
        FIELD_sourceNode,
        FIELD_destNode,
        FIELD_sequenceNum,
        FIELD_creationTime,
        FIELD_ttl,
        FIELD_hopCount,
        FIELD_retransmission,
        FIELD_payload,
    };
  public:
    EnhancedNetMsgDescriptor();
    virtual ~EnhancedNetMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(EnhancedNetMsgDescriptor)

EnhancedNetMsgDescriptor::EnhancedNetMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(EnhancedNetMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

EnhancedNetMsgDescriptor::~EnhancedNetMsgDescriptor()
{
    delete[] propertyNames;
}

bool EnhancedNetMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<EnhancedNetMsg *>(obj)!=nullptr;
}

const char **EnhancedNetMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *EnhancedNetMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int EnhancedNetMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 9+base->getFieldCount() : 9;
}

unsigned int EnhancedNetMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_messageId
        FD_ISEDITABLE,    // FIELD_sourceNode
        FD_ISEDITABLE,    // FIELD_destNode
        FD_ISEDITABLE,    // FIELD_sequenceNum
        FD_ISEDITABLE,    // FIELD_creationTime
        FD_ISEDITABLE,    // FIELD_ttl
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_retransmission
        FD_ISEDITABLE,    // FIELD_payload
    };
    return (field >= 0 && field < 9) ? fieldTypeFlags[field] : 0;
}

const char *EnhancedNetMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "messageId",
        "sourceNode",
        "destNode",
        "sequenceNum",
        "creationTime",
        "ttl",
        "hopCount",
        "retransmission",
        "payload",
    };
    return (field >= 0 && field < 9) ? fieldNames[field] : nullptr;
}

int EnhancedNetMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "messageId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "sourceNode") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "destNode") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNum") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "creationTime") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "ttl") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "retransmission") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "payload") == 0) return baseIndex + 8;
    return base ? base->findField(fieldName) : -1;
}

const char *EnhancedNetMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_messageId
        "int",    // FIELD_sourceNode
        "int",    // FIELD_destNode
        "int",    // FIELD_sequenceNum
        "omnetpp::simtime_t",    // FIELD_creationTime
        "int",    // FIELD_ttl
        "int",    // FIELD_hopCount
        "bool",    // FIELD_retransmission
        "string",    // FIELD_payload
    };
    return (field >= 0 && field < 9) ? fieldTypeStrings[field] : nullptr;
}

const char **EnhancedNetMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *EnhancedNetMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int EnhancedNetMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    EnhancedNetMsg *pp = omnetpp::fromAnyPtr<EnhancedNetMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void EnhancedNetMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    EnhancedNetMsg *pp = omnetpp::fromAnyPtr<EnhancedNetMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'EnhancedNetMsg'", field);
    }
}

const char *EnhancedNetMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    EnhancedNetMsg *pp = omnetpp::fromAnyPtr<EnhancedNetMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string EnhancedNetMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    EnhancedNetMsg *pp = omnetpp::fromAnyPtr<EnhancedNetMsg>(object); (void)pp;
    switch (field) {
        case FIELD_messageId: return long2string(pp->getMessageId());
        case FIELD_sourceNode: return long2string(pp->getSourceNode());
        case FIELD_destNode: return long2string(pp->getDestNode());
        case FIELD_sequenceNum: return long2string(pp->getSequenceNum());
        case FIELD_creationTime: return simtime2string(pp->getCreationTime());
        case FIELD_ttl: return long2string(pp->getTtl());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_retransmission: return bool2string(pp->getRetransmission());
        case FIELD_payload: return oppstring2string(pp->getPayload());
        default: return "";
    }
}

void EnhancedNetMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    EnhancedNetMsg *pp = omnetpp::fromAnyPtr<EnhancedNetMsg>(object); (void)pp;
    switch (field) {
        case FIELD_messageId: pp->setMessageId(string2long(value)); break;
        case FIELD_sourceNode: pp->setSourceNode(string2long(value)); break;
        case FIELD_destNode: pp->setDestNode(string2long(value)); break;
        case FIELD_sequenceNum: pp->setSequenceNum(string2long(value)); break;
        case FIELD_creationTime: pp->setCreationTime(string2simtime(value)); break;
        case FIELD_ttl: pp->setTtl(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_retransmission: pp->setRetransmission(string2bool(value)); break;
        case FIELD_payload: pp->setPayload((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EnhancedNetMsg'", field);
    }
}

omnetpp::cValue EnhancedNetMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    EnhancedNetMsg *pp = omnetpp::fromAnyPtr<EnhancedNetMsg>(object); (void)pp;
    switch (field) {
        case FIELD_messageId: return pp->getMessageId();
        case FIELD_sourceNode: return pp->getSourceNode();
        case FIELD_destNode: return pp->getDestNode();
        case FIELD_sequenceNum: return pp->getSequenceNum();
        case FIELD_creationTime: return pp->getCreationTime().dbl();
        case FIELD_ttl: return pp->getTtl();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_retransmission: return pp->getRetransmission();
        case FIELD_payload: return pp->getPayload();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'EnhancedNetMsg' as cValue -- field index out of range?", field);
    }
}

void EnhancedNetMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    EnhancedNetMsg *pp = omnetpp::fromAnyPtr<EnhancedNetMsg>(object); (void)pp;
    switch (field) {
        case FIELD_messageId: pp->setMessageId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceNode: pp->setSourceNode(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destNode: pp->setDestNode(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNum: pp->setSequenceNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_creationTime: pp->setCreationTime(value.doubleValue()); break;
        case FIELD_ttl: pp->setTtl(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_retransmission: pp->setRetransmission(value.boolValue()); break;
        case FIELD_payload: pp->setPayload(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EnhancedNetMsg'", field);
    }
}

const char *EnhancedNetMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr EnhancedNetMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    EnhancedNetMsg *pp = omnetpp::fromAnyPtr<EnhancedNetMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void EnhancedNetMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    EnhancedNetMsg *pp = omnetpp::fromAnyPtr<EnhancedNetMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EnhancedNetMsg'", field);
    }
}

Register_Class(AckMessage)

AckMessage::AckMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

AckMessage::AckMessage(const AckMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

AckMessage::~AckMessage()
{
}

AckMessage& AckMessage::operator=(const AckMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void AckMessage::copy(const AckMessage& other)
{
    this->ackId = other.ackId;
    this->sourceNode = other.sourceNode;
    this->destNode = other.destNode;
    this->sequenceNum = other.sequenceNum;
    this->sendTime = other.sendTime;
}

void AckMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->ackId);
    doParsimPacking(b,this->sourceNode);
    doParsimPacking(b,this->destNode);
    doParsimPacking(b,this->sequenceNum);
    doParsimPacking(b,this->sendTime);
}

void AckMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->ackId);
    doParsimUnpacking(b,this->sourceNode);
    doParsimUnpacking(b,this->destNode);
    doParsimUnpacking(b,this->sequenceNum);
    doParsimUnpacking(b,this->sendTime);
}

int AckMessage::getAckId() const
{
    return this->ackId;
}

void AckMessage::setAckId(int ackId)
{
    this->ackId = ackId;
}

int AckMessage::getSourceNode() const
{
    return this->sourceNode;
}

void AckMessage::setSourceNode(int sourceNode)
{
    this->sourceNode = sourceNode;
}

int AckMessage::getDestNode() const
{
    return this->destNode;
}

void AckMessage::setDestNode(int destNode)
{
    this->destNode = destNode;
}

int AckMessage::getSequenceNum() const
{
    return this->sequenceNum;
}

void AckMessage::setSequenceNum(int sequenceNum)
{
    this->sequenceNum = sequenceNum;
}

omnetpp::simtime_t AckMessage::getSendTime() const
{
    return this->sendTime;
}

void AckMessage::setSendTime(omnetpp::simtime_t sendTime)
{
    this->sendTime = sendTime;
}

class AckMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_ackId,
        FIELD_sourceNode,
        FIELD_destNode,
        FIELD_sequenceNum,
        FIELD_sendTime,
    };
  public:
    AckMessageDescriptor();
    virtual ~AckMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(AckMessageDescriptor)

AckMessageDescriptor::AckMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(AckMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

AckMessageDescriptor::~AckMessageDescriptor()
{
    delete[] propertyNames;
}

bool AckMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<AckMessage *>(obj)!=nullptr;
}

const char **AckMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *AckMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int AckMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int AckMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_ackId
        FD_ISEDITABLE,    // FIELD_sourceNode
        FD_ISEDITABLE,    // FIELD_destNode
        FD_ISEDITABLE,    // FIELD_sequenceNum
        FD_ISEDITABLE,    // FIELD_sendTime
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *AckMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "ackId",
        "sourceNode",
        "destNode",
        "sequenceNum",
        "sendTime",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int AckMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "ackId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "sourceNode") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "destNode") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNum") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *AckMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_ackId
        "int",    // FIELD_sourceNode
        "int",    // FIELD_destNode
        "int",    // FIELD_sequenceNum
        "omnetpp::simtime_t",    // FIELD_sendTime
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **AckMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *AckMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int AckMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    AckMessage *pp = omnetpp::fromAnyPtr<AckMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void AckMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    AckMessage *pp = omnetpp::fromAnyPtr<AckMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'AckMessage'", field);
    }
}

const char *AckMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    AckMessage *pp = omnetpp::fromAnyPtr<AckMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string AckMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    AckMessage *pp = omnetpp::fromAnyPtr<AckMessage>(object); (void)pp;
    switch (field) {
        case FIELD_ackId: return long2string(pp->getAckId());
        case FIELD_sourceNode: return long2string(pp->getSourceNode());
        case FIELD_destNode: return long2string(pp->getDestNode());
        case FIELD_sequenceNum: return long2string(pp->getSequenceNum());
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        default: return "";
    }
}

void AckMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AckMessage *pp = omnetpp::fromAnyPtr<AckMessage>(object); (void)pp;
    switch (field) {
        case FIELD_ackId: pp->setAckId(string2long(value)); break;
        case FIELD_sourceNode: pp->setSourceNode(string2long(value)); break;
        case FIELD_destNode: pp->setDestNode(string2long(value)); break;
        case FIELD_sequenceNum: pp->setSequenceNum(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AckMessage'", field);
    }
}

omnetpp::cValue AckMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    AckMessage *pp = omnetpp::fromAnyPtr<AckMessage>(object); (void)pp;
    switch (field) {
        case FIELD_ackId: return pp->getAckId();
        case FIELD_sourceNode: return pp->getSourceNode();
        case FIELD_destNode: return pp->getDestNode();
        case FIELD_sequenceNum: return pp->getSequenceNum();
        case FIELD_sendTime: return pp->getSendTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'AckMessage' as cValue -- field index out of range?", field);
    }
}

void AckMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AckMessage *pp = omnetpp::fromAnyPtr<AckMessage>(object); (void)pp;
    switch (field) {
        case FIELD_ackId: pp->setAckId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceNode: pp->setSourceNode(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destNode: pp->setDestNode(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNum: pp->setSequenceNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AckMessage'", field);
    }
}

const char *AckMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr AckMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    AckMessage *pp = omnetpp::fromAnyPtr<AckMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void AckMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    AckMessage *pp = omnetpp::fromAnyPtr<AckMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AckMessage'", field);
    }
}

Register_Class(RouteUpdateMsg)

RouteUpdateMsg::RouteUpdateMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

RouteUpdateMsg::RouteUpdateMsg(const RouteUpdateMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

RouteUpdateMsg::~RouteUpdateMsg()
{
    delete [] this->routeEntries;
}

RouteUpdateMsg& RouteUpdateMsg::operator=(const RouteUpdateMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void RouteUpdateMsg::copy(const RouteUpdateMsg& other)
{
    this->updateId = other.updateId;
    this->sourceNode = other.sourceNode;
    this->updateTime = other.updateTime;
    this->destCount = other.destCount;
    delete [] this->routeEntries;
    this->routeEntries = (other.routeEntries_arraysize==0) ? nullptr : new omnetpp::opp_string[other.routeEntries_arraysize];
    routeEntries_arraysize = other.routeEntries_arraysize;
    for (size_t i = 0; i < routeEntries_arraysize; i++) {
        this->routeEntries[i] = other.routeEntries[i];
    }
}

void RouteUpdateMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->updateId);
    doParsimPacking(b,this->sourceNode);
    doParsimPacking(b,this->updateTime);
    doParsimPacking(b,this->destCount);
    b->pack(routeEntries_arraysize);
    doParsimArrayPacking(b,this->routeEntries,routeEntries_arraysize);
}

void RouteUpdateMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->updateId);
    doParsimUnpacking(b,this->sourceNode);
    doParsimUnpacking(b,this->updateTime);
    doParsimUnpacking(b,this->destCount);
    delete [] this->routeEntries;
    b->unpack(routeEntries_arraysize);
    if (routeEntries_arraysize == 0) {
        this->routeEntries = nullptr;
    } else {
        this->routeEntries = new omnetpp::opp_string[routeEntries_arraysize];
        doParsimArrayUnpacking(b,this->routeEntries,routeEntries_arraysize);
    }
}

int RouteUpdateMsg::getUpdateId() const
{
    return this->updateId;
}

void RouteUpdateMsg::setUpdateId(int updateId)
{
    this->updateId = updateId;
}

int RouteUpdateMsg::getSourceNode() const
{
    return this->sourceNode;
}

void RouteUpdateMsg::setSourceNode(int sourceNode)
{
    this->sourceNode = sourceNode;
}

omnetpp::simtime_t RouteUpdateMsg::getUpdateTime() const
{
    return this->updateTime;
}

void RouteUpdateMsg::setUpdateTime(omnetpp::simtime_t updateTime)
{
    this->updateTime = updateTime;
}

int RouteUpdateMsg::getDestCount() const
{
    return this->destCount;
}

void RouteUpdateMsg::setDestCount(int destCount)
{
    this->destCount = destCount;
}

size_t RouteUpdateMsg::getRouteEntriesArraySize() const
{
    return routeEntries_arraysize;
}

const char * RouteUpdateMsg::getRouteEntries(size_t k) const
{
    if (k >= routeEntries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)routeEntries_arraysize, (unsigned long)k);
    return this->routeEntries[k].c_str();
}

void RouteUpdateMsg::setRouteEntriesArraySize(size_t newSize)
{
    omnetpp::opp_string *routeEntries2 = (newSize==0) ? nullptr : new omnetpp::opp_string[newSize];
    size_t minSize = routeEntries_arraysize < newSize ? routeEntries_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        routeEntries2[i] = this->routeEntries[i];
    delete [] this->routeEntries;
    this->routeEntries = routeEntries2;
    routeEntries_arraysize = newSize;
}

void RouteUpdateMsg::setRouteEntries(size_t k, const char * routeEntries)
{
    if (k >= routeEntries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)routeEntries_arraysize, (unsigned long)k);
    this->routeEntries[k] = routeEntries;
}

void RouteUpdateMsg::insertRouteEntries(size_t k, const char * routeEntries)
{
    if (k > routeEntries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)routeEntries_arraysize, (unsigned long)k);
    size_t newSize = routeEntries_arraysize + 1;
    omnetpp::opp_string *routeEntries2 = new omnetpp::opp_string[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        routeEntries2[i] = this->routeEntries[i];
    routeEntries2[k] = routeEntries;
    for (i = k + 1; i < newSize; i++)
        routeEntries2[i] = this->routeEntries[i-1];
    delete [] this->routeEntries;
    this->routeEntries = routeEntries2;
    routeEntries_arraysize = newSize;
}

void RouteUpdateMsg::appendRouteEntries(const char * routeEntries)
{
    insertRouteEntries(routeEntries_arraysize, routeEntries);
}

void RouteUpdateMsg::eraseRouteEntries(size_t k)
{
    if (k >= routeEntries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)routeEntries_arraysize, (unsigned long)k);
    size_t newSize = routeEntries_arraysize - 1;
    omnetpp::opp_string *routeEntries2 = (newSize == 0) ? nullptr : new omnetpp::opp_string[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        routeEntries2[i] = this->routeEntries[i];
    for (i = k; i < newSize; i++)
        routeEntries2[i] = this->routeEntries[i+1];
    delete [] this->routeEntries;
    this->routeEntries = routeEntries2;
    routeEntries_arraysize = newSize;
}

class RouteUpdateMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_updateId,
        FIELD_sourceNode,
        FIELD_updateTime,
        FIELD_destCount,
        FIELD_routeEntries,
    };
  public:
    RouteUpdateMsgDescriptor();
    virtual ~RouteUpdateMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RouteUpdateMsgDescriptor)

RouteUpdateMsgDescriptor::RouteUpdateMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(RouteUpdateMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

RouteUpdateMsgDescriptor::~RouteUpdateMsgDescriptor()
{
    delete[] propertyNames;
}

bool RouteUpdateMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RouteUpdateMsg *>(obj)!=nullptr;
}

const char **RouteUpdateMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RouteUpdateMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RouteUpdateMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int RouteUpdateMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_updateId
        FD_ISEDITABLE,    // FIELD_sourceNode
        FD_ISEDITABLE,    // FIELD_updateTime
        FD_ISEDITABLE,    // FIELD_destCount
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_routeEntries
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *RouteUpdateMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "updateId",
        "sourceNode",
        "updateTime",
        "destCount",
        "routeEntries",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int RouteUpdateMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "updateId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "sourceNode") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "updateTime") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "destCount") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "routeEntries") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *RouteUpdateMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_updateId
        "int",    // FIELD_sourceNode
        "omnetpp::simtime_t",    // FIELD_updateTime
        "int",    // FIELD_destCount
        "string",    // FIELD_routeEntries
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **RouteUpdateMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RouteUpdateMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RouteUpdateMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RouteUpdateMsg *pp = omnetpp::fromAnyPtr<RouteUpdateMsg>(object); (void)pp;
    switch (field) {
        case FIELD_routeEntries: return pp->getRouteEntriesArraySize();
        default: return 0;
    }
}

void RouteUpdateMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RouteUpdateMsg *pp = omnetpp::fromAnyPtr<RouteUpdateMsg>(object); (void)pp;
    switch (field) {
        case FIELD_routeEntries: pp->setRouteEntriesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RouteUpdateMsg'", field);
    }
}

const char *RouteUpdateMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RouteUpdateMsg *pp = omnetpp::fromAnyPtr<RouteUpdateMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RouteUpdateMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RouteUpdateMsg *pp = omnetpp::fromAnyPtr<RouteUpdateMsg>(object); (void)pp;
    switch (field) {
        case FIELD_updateId: return long2string(pp->getUpdateId());
        case FIELD_sourceNode: return long2string(pp->getSourceNode());
        case FIELD_updateTime: return simtime2string(pp->getUpdateTime());
        case FIELD_destCount: return long2string(pp->getDestCount());
        case FIELD_routeEntries: return oppstring2string(pp->getRouteEntries(i));
        default: return "";
    }
}

void RouteUpdateMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RouteUpdateMsg *pp = omnetpp::fromAnyPtr<RouteUpdateMsg>(object); (void)pp;
    switch (field) {
        case FIELD_updateId: pp->setUpdateId(string2long(value)); break;
        case FIELD_sourceNode: pp->setSourceNode(string2long(value)); break;
        case FIELD_updateTime: pp->setUpdateTime(string2simtime(value)); break;
        case FIELD_destCount: pp->setDestCount(string2long(value)); break;
        case FIELD_routeEntries: pp->setRouteEntries(i,(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RouteUpdateMsg'", field);
    }
}

omnetpp::cValue RouteUpdateMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RouteUpdateMsg *pp = omnetpp::fromAnyPtr<RouteUpdateMsg>(object); (void)pp;
    switch (field) {
        case FIELD_updateId: return pp->getUpdateId();
        case FIELD_sourceNode: return pp->getSourceNode();
        case FIELD_updateTime: return pp->getUpdateTime().dbl();
        case FIELD_destCount: return pp->getDestCount();
        case FIELD_routeEntries: return pp->getRouteEntries(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RouteUpdateMsg' as cValue -- field index out of range?", field);
    }
}

void RouteUpdateMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RouteUpdateMsg *pp = omnetpp::fromAnyPtr<RouteUpdateMsg>(object); (void)pp;
    switch (field) {
        case FIELD_updateId: pp->setUpdateId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceNode: pp->setSourceNode(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_updateTime: pp->setUpdateTime(value.doubleValue()); break;
        case FIELD_destCount: pp->setDestCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_routeEntries: pp->setRouteEntries(i,value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RouteUpdateMsg'", field);
    }
}

const char *RouteUpdateMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr RouteUpdateMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RouteUpdateMsg *pp = omnetpp::fromAnyPtr<RouteUpdateMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RouteUpdateMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RouteUpdateMsg *pp = omnetpp::fromAnyPtr<RouteUpdateMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RouteUpdateMsg'", field);
    }
}

Register_Class(HeartbeatMsg)

HeartbeatMsg::HeartbeatMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

HeartbeatMsg::HeartbeatMsg(const HeartbeatMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

HeartbeatMsg::~HeartbeatMsg()
{
}

HeartbeatMsg& HeartbeatMsg::operator=(const HeartbeatMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void HeartbeatMsg::copy(const HeartbeatMsg& other)
{
    this->heartbeatId = other.heartbeatId;
    this->sourceNode = other.sourceNode;
    this->targetNode = other.targetNode;
    this->sendTime = other.sendTime;
    this->isAlive_ = other.isAlive_;
}

void HeartbeatMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->heartbeatId);
    doParsimPacking(b,this->sourceNode);
    doParsimPacking(b,this->targetNode);
    doParsimPacking(b,this->sendTime);
    doParsimPacking(b,this->isAlive_);
}

void HeartbeatMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->heartbeatId);
    doParsimUnpacking(b,this->sourceNode);
    doParsimUnpacking(b,this->targetNode);
    doParsimUnpacking(b,this->sendTime);
    doParsimUnpacking(b,this->isAlive_);
}

int HeartbeatMsg::getHeartbeatId() const
{
    return this->heartbeatId;
}

void HeartbeatMsg::setHeartbeatId(int heartbeatId)
{
    this->heartbeatId = heartbeatId;
}

int HeartbeatMsg::getSourceNode() const
{
    return this->sourceNode;
}

void HeartbeatMsg::setSourceNode(int sourceNode)
{
    this->sourceNode = sourceNode;
}

int HeartbeatMsg::getTargetNode() const
{
    return this->targetNode;
}

void HeartbeatMsg::setTargetNode(int targetNode)
{
    this->targetNode = targetNode;
}

omnetpp::simtime_t HeartbeatMsg::getSendTime() const
{
    return this->sendTime;
}

void HeartbeatMsg::setSendTime(omnetpp::simtime_t sendTime)
{
    this->sendTime = sendTime;
}

bool HeartbeatMsg::isAlive() const
{
    return this->isAlive_;
}

void HeartbeatMsg::setIsAlive(bool isAlive)
{
    this->isAlive_ = isAlive;
}

class HeartbeatMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_heartbeatId,
        FIELD_sourceNode,
        FIELD_targetNode,
        FIELD_sendTime,
        FIELD_isAlive,
    };
  public:
    HeartbeatMsgDescriptor();
    virtual ~HeartbeatMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(HeartbeatMsgDescriptor)

HeartbeatMsgDescriptor::HeartbeatMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(HeartbeatMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

HeartbeatMsgDescriptor::~HeartbeatMsgDescriptor()
{
    delete[] propertyNames;
}

bool HeartbeatMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<HeartbeatMsg *>(obj)!=nullptr;
}

const char **HeartbeatMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *HeartbeatMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int HeartbeatMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int HeartbeatMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_heartbeatId
        FD_ISEDITABLE,    // FIELD_sourceNode
        FD_ISEDITABLE,    // FIELD_targetNode
        FD_ISEDITABLE,    // FIELD_sendTime
        FD_ISEDITABLE,    // FIELD_isAlive
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *HeartbeatMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "heartbeatId",
        "sourceNode",
        "targetNode",
        "sendTime",
        "isAlive",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int HeartbeatMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "heartbeatId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "sourceNode") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "targetNode") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "isAlive") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *HeartbeatMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_heartbeatId
        "int",    // FIELD_sourceNode
        "int",    // FIELD_targetNode
        "omnetpp::simtime_t",    // FIELD_sendTime
        "bool",    // FIELD_isAlive
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **HeartbeatMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *HeartbeatMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int HeartbeatMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    HeartbeatMsg *pp = omnetpp::fromAnyPtr<HeartbeatMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void HeartbeatMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    HeartbeatMsg *pp = omnetpp::fromAnyPtr<HeartbeatMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'HeartbeatMsg'", field);
    }
}

const char *HeartbeatMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    HeartbeatMsg *pp = omnetpp::fromAnyPtr<HeartbeatMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string HeartbeatMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    HeartbeatMsg *pp = omnetpp::fromAnyPtr<HeartbeatMsg>(object); (void)pp;
    switch (field) {
        case FIELD_heartbeatId: return long2string(pp->getHeartbeatId());
        case FIELD_sourceNode: return long2string(pp->getSourceNode());
        case FIELD_targetNode: return long2string(pp->getTargetNode());
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        case FIELD_isAlive: return bool2string(pp->isAlive());
        default: return "";
    }
}

void HeartbeatMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    HeartbeatMsg *pp = omnetpp::fromAnyPtr<HeartbeatMsg>(object); (void)pp;
    switch (field) {
        case FIELD_heartbeatId: pp->setHeartbeatId(string2long(value)); break;
        case FIELD_sourceNode: pp->setSourceNode(string2long(value)); break;
        case FIELD_targetNode: pp->setTargetNode(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        case FIELD_isAlive: pp->setIsAlive(string2bool(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'HeartbeatMsg'", field);
    }
}

omnetpp::cValue HeartbeatMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    HeartbeatMsg *pp = omnetpp::fromAnyPtr<HeartbeatMsg>(object); (void)pp;
    switch (field) {
        case FIELD_heartbeatId: return pp->getHeartbeatId();
        case FIELD_sourceNode: return pp->getSourceNode();
        case FIELD_targetNode: return pp->getTargetNode();
        case FIELD_sendTime: return pp->getSendTime().dbl();
        case FIELD_isAlive: return pp->isAlive();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'HeartbeatMsg' as cValue -- field index out of range?", field);
    }
}

void HeartbeatMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    HeartbeatMsg *pp = omnetpp::fromAnyPtr<HeartbeatMsg>(object); (void)pp;
    switch (field) {
        case FIELD_heartbeatId: pp->setHeartbeatId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceNode: pp->setSourceNode(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_targetNode: pp->setTargetNode(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        case FIELD_isAlive: pp->setIsAlive(value.boolValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'HeartbeatMsg'", field);
    }
}

const char *HeartbeatMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr HeartbeatMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    HeartbeatMsg *pp = omnetpp::fromAnyPtr<HeartbeatMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void HeartbeatMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    HeartbeatMsg *pp = omnetpp::fromAnyPtr<HeartbeatMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'HeartbeatMsg'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

