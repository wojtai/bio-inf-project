//
// Created by wojtek on 23.03.19.
//

#include "Element.h"

//TODO unit tests on this class

Element::Element(std::string s) : SimpleElement(s) {
    this->parts = std::vector<SimpleElement>();
    this->parts.emplace_back(SimpleElement(s));
}

Element::~Element() {
    this->parts.clear();
    this->parts.shrink_to_fit();
    //delete this->parts;
}

std::vector<SimpleElement> * Element::getParts() {
    return &this->parts;
}

bool Element::appendElement(Element e, unsigned int p) {
    //try to append element e on p common nucleotides
    std::string s1 = this->getTail(p);
    std::string s2 = e.getHead(p);
    if(s1 == s2){
        //this->value += e.getTail((int)e.getValue().size()-p);
        this->value += e.getValue().substr(p,e.getValue().size()-p);
        this->size += e.getSize();
        this->parts.insert(this->parts.end(), e.getParts()->begin(), e.getParts()->end());
//        for(const auto & a : *e.getParts()){
//            this->parts.push_back(a);
//        }
        return true;
    } else {
        return false;
    }

}

SimpleElement::SimpleElement(std::string s) {
    this->value = s;
    this->size = 1;
}

std::string SimpleElement::getValue() {
    return this->value;
}

int SimpleElement::getSize() {
    return this->size;
}

std::vector<SimpleElement> * SimpleElement::getParts() {
    //return std::vector<SimpleElement>(); //return simple vector
    return nullptr;
}

const std::string SimpleElement::getHead(unsigned int p) {
    return this->value.substr(0,p);
}

const std::string SimpleElement::getTail(unsigned int p) {
    //return this->value.substr(std::max(0, (int)this->value.size()-(int)p), (unsigned int)this->value.size());
    return this->value.substr(std::max(0, (int)this->value.size()-(int)p));
}

int SimpleElement::appendSize(SimpleElement se) {
    unsigned int l = std::min(se.value.size(), this->value.size())-1;
    while(l>0){
        if(se.getHead(l) == this->getTail(l)) return l;
        l--;
    }
    return 0;
}

std::string SimpleElement::getValueFromParts() {
    return this->value;
}

int Element::calculateTotalLengthOfParts() {
    int result = this->parts[0].getValue().size();
    for(unsigned long i=1; i<this->parts.size(); i++){
        result += (this->parts[i].getValue().size()-this->parts[i-1].appendSize(parts[i]));
    }
    return result;
}

std::string Element::getValueFromParts() {
    Element begin = Element(parts[0].getValue());
    for(unsigned long i=1; i<parts.size(); i++){
        Element temp = Element(parts[i].getValue());
        begin.appendElement(temp, begin.appendSize(temp));
    }
    return begin.getValue();
}

void Element::trimToSize(unsigned long size) {
    while(this->value.size() > size){
        int l1 = this->parts[0].appendSize(this->parts[1]); //check connection from front
        int l2 = this->parts[this->size-2].appendSize(this->parts[this->size-1]); //check connection from back
        if(l1 < l2){
            this->parts.erase(this->parts.begin(), this->parts.begin()+1); //remove first
        } else if (l2 <= l1){
            this->parts.erase(this->parts.end()-1, this->parts.end()); //remove last
        }
        this->value = this->getValueFromParts();
        this->size--;
    }
}

//std::string Element::getHead(unsigned int p) {
//    return this->parts.front().getHead(p);
//}
//
//std::string Element::getTail(unsigned int p) {
//    return this->parts.back().getTail(p);
//}