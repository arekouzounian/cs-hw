#include "labellist.h"

#include <cassert>

/// just in case the compiler doesn't have it
typedef unsigned int uint;

// Complete - Do not alter
LabelList::MsgNode::MsgNode(const MsgType& msg)
    : msg_(msg), labelled_(std::vector<bool>()), next_(NodeVec()), prev_(NodeVec()) {
    // Every message is part of the "all" (index 0) label
    labelled_.push_back(true);
    next_.push_back(nullptr);
    prev_.push_back(nullptr);
}

// Complete - Do not alter
LabelList::MsgToken::MsgToken() : node_(nullptr), list_(nullptr) {}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::MsgToken(MsgNode* node, LabelList* list)
// Add any initializers here
    : node_(node), list_(list) {
}

// To be completed - must adhere to any requirements in the .h file
bool LabelList::MsgToken::operator==(const MsgToken& rhs) const {
    // check if the current MsgToken's node points to the same node as rhs
    // if yes, then return true
    // if not, return false
    return this->node_ == rhs.node_;
}

// Complete - Do not alter
bool LabelList::MsgToken::operator!=(const MsgToken& rhs) const { return !operator==(rhs); }

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::operator bool() const {
    // according to the .h file, this method will return true if the token
    // points to a valid MsgNode.
    // The case where the MsgNode is invalid means that it wasn't set/constructed
    // in the constructor, thus it would be the nullptr.
    // Therefore we can simply check if the MsgNode pointer is nullptr

    return this->node_ != nullptr;
}

// Complete - Do not alter
bool LabelList::MsgToken::isValid() const { return this->node_ != nullptr; }

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::next(const LabelType& label) {
    // Add your code here
    size_t index = this->list_->findLabelIndex(label);
    if (false == this->isLabelMember(index))
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->next_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::prev(const LabelType& label) {
    // Add your code here
    size_t index = this->list_->findLabelIndex(label);
    if (false == this->isLabelMember(index))
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->prev_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(const LabelType& label) const
{
    size_t index = this->list_->findLabelIndex(label);
    return isLabelMember(index);
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(size_t index) const
{
    return this->isValid() &&
        index != INVALID_LABEL &&
        index < this->node_->labelled_.size() &&
        this->node_->labelled_[index];
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType const& LabelList::MsgToken::msg() const {
    if (!(this->node_)) throw std::logic_error("Invalid Message");
    return this->node_->msg_;
}
// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType& LabelList::MsgToken::msg() {
    if (!(this->node_)) throw std::logic_error("Invalid Message");
    return this->node_->msg_;
}

// To be completed - must adhere to any requirements in the .h file
std::ostream& operator<<(std::ostream& ostr, const LabelList::MsgToken& token) {
    if (token.isValid())
    {
        ostr << token.node_->msg_;
    }
    return ostr;
}

// Static constant
const LabelList::MsgToken LabelList::end_(nullptr, nullptr);

// To be completed - must adhere to any requirements in the .h file
LabelList::LabelList()
// Add any initializers here
    : heads_(NodeVec()), labels_(std::vector<LabelType>()), tail_(nullptr)
{
    labels_.push_back("all");
    heads_.push_back(nullptr);
}

// To be completed - must adhere to any requirements in the .h file
LabelList::~LabelList()
{
    this->clear();
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::clear()
{
    if (this->empty()) return;
    //loop through each token and remove it 
    MsgNode* head = this->heads_[0];
    //worst-case O(n)
    while ( head != nullptr )
    {
        MsgNode* curr = head;
        head = head->next_[0];
        delete curr;
    }
    //resize make sure tail is null, heads are nullptr
    this->tail_ = nullptr;
    for(int i = 0; i < this->heads_.size(); i++)
    {
        this->heads_[i] = nullptr;
    }
}


// To be completed - must adhere to any requirements in the .h file
bool LabelList::empty() const {
    //covers case where the list 
    return this->heads_.size() < 1 || !this->heads_[0];
}

// Complete
size_t LabelList::size() const {
    MsgNode* n = this->heads_[0];
    size_t cnt = 0;
    while (n != nullptr)
    {
        cnt++;
        n = n->next_[0];
    }
    return cnt;
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken LabelList::add(const MsgType& msg) {
    // create msgnode around given message
    MsgNode* msg_node_ptr = new MsgNode(msg);
    // create a MsgToken aronud the msg_node
    MsgToken msg_container(msg_node_ptr, this);

    // add the "all" label to new message

    if (tail_ == nullptr)
    {
        // in this case, the list is empty 
        // add our new head to the label list
        heads_[0] = msg_node_ptr;

        // set our tail pointer to point to the newly created node
        tail_ = msg_node_ptr;
    }
    else
    {
        tail_->next_[0] = msg_node_ptr;
        msg_node_ptr->prev_[0] = tail_;
        tail_ = msg_node_ptr;
    }

    return msg_container;
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::remove(const MsgToken& token)
{
    if (!token.isValid())
        return;

    //for each label the node has, we unlabel it 
    //O(|L|)
    for (size_t i = 0; i < token.node_->labelled_.size(); i++)
    {
        //O(1)
        if (token.node_->labelled_[i])
        {
            //case 1: left and right existent (intermediary deletion)
            //case 2: no left, right exists (change head)
            //case 3: left exists, no right (change tail)
            //case 4: no linkage to be done (empty list)
            MsgNode* left = token.node_->prev_[i];
            MsgNode* right = token.node_->next_[i];
            if (left && right)
            {
                left->next_[i] = right;
                right->prev_[i] = left;
            }
            else if (!left && right)
            {
                right->prev_[i] = nullptr;
                token.list_->heads_[i] = right;
            }
            else if (left && !right)
            {
                left->next_[i] = nullptr;
                if (i == 0) token.list_->tail_ = left;
            }
            else
            {
                token.list_->heads_[i] = nullptr;
                if (i == 0) token.list_->tail_ = nullptr;
            }
        }
    }

    // Finally, we delete the MsgNode that is referenced by the token
    delete token.node_;
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::label(const MsgToken& token, const LabelType& label) {
    if (!token.isValid()) //O(1)
    {
        throw std::invalid_argument("Bad token in label()");
    }

    //find or add label index from LabelList
    size_t lbl_index = findLabelIndex(label); //O(L)

    //error checks: check whether it's already labelled or 
    //if label doesn't exist 
    if (token.isLabelMember(lbl_index)) { return; } //O(1)
    else if (lbl_index == INVALID_LABEL)
    {
        labels_.push_back(label);
        heads_.push_back(token.node_);
        lbl_index = labels_.size() - 1;
    }

    //now we label the message node accordingly
    //fill in the intermediary levels, if they exist 
    for (uint i = token.node_->labelled_.size() - 1; i < lbl_index; i++)
    {
        token.node_->labelled_.push_back(false);
        token.node_->next_.push_back(nullptr);
        token.node_->prev_.push_back(nullptr);
    }
    token.node_->labelled_[lbl_index] = true;

    MsgNode* currLeft = token.node_->prev_[0];
    MsgNode* currRight = token.node_->next_[0];

    //Loop through left side of list to find immediate left
    //Worst-case O(n)
    while (currLeft != nullptr && !currLeft->hasLabel(lbl_index))
    {
        currLeft = currLeft->prev_[0];
    }

    //Similarly loop through right side of list to find immediate right
    //Worst-case O(n)
    while (currRight != nullptr && !currRight->hasLabel(lbl_index))
    {
        currRight = currRight->next_[0];
    }

    //case 1: left null, right not (insert new head)
    //case 2: right null, left not (insert new tail)
    //case 3: both populated (intermediary insertion)
    //case 4: both null, but this case is already taken care of up above where we add a new label
    if (!currLeft && currRight)
    {
        token.node_->next_[lbl_index] = currRight;
        currRight->prev_[lbl_index] = token.node_;
        token.list_->heads_[lbl_index] = token.node_;
    }
    else if (currLeft && !currRight)
    {
        token.node_->prev_[lbl_index] = currLeft;
        currLeft->next_[lbl_index] = token.node_;
    }
    else if (currLeft && currRight)
    {
        token.node_->next_[lbl_index] = currRight;
        token.node_->prev_[lbl_index] = currLeft;
        currLeft->next_[lbl_index] = token.node_;
        currRight->prev_[lbl_index] = token.node_;
    }
    else
    {
        token.list_->heads_[lbl_index] = token.node_;
    }
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::unlabel(const MsgToken& token, const LabelType& label) {
    if (!token.isValid())
    {
        throw std::invalid_argument("Bad token in label()");
    }

    //O(|L|), throws out of range as we need
    size_t lbl_index = getLabelIndex(label);
    //does nothing if it's the 'all' label
    if (lbl_index == 0 || !token.isLabelMember(lbl_index))
        return;

    //at this point, we know that it's a valid token, 
    //a valid label other than 'all,' and the given token
    //is a member of that label's list. 

    MsgNode* left = token.node_->prev_[lbl_index];
    MsgNode* right = token.node_->next_[lbl_index];

    //Four cases
    //case 1: left and right are both valid (intermediary deletion)
    //case 2: left null, right existent (changing head)
    //case 3: left existent, right null (changing tail)
    //case 4: left & right null (empty the list) 
    if (left && right)
    {
        left->next_[lbl_index] = right;
        right->prev_[lbl_index] = left;
    }
    else if (!left && right)
    {
        token.list_->heads_[lbl_index] = right;
        right->prev_[lbl_index] = nullptr;
    }
    else if (left && !right)
    {
        left->next_[lbl_index] = nullptr;
    }
    else
    {
        token.list_->heads_[lbl_index] = nullptr;
    }

    //all that's left is to make sure that our token is labelled correctly
    token.node_->labelled_[lbl_index] = false;
    token.node_->next_[lbl_index] = nullptr;
    token.node_->prev_[lbl_index] = nullptr;
}

// Complete - Do not alter
LabelList::MsgToken LabelList::find(const MsgType& msg, const LabelType& label) {
    // See if the label exists, will throw if not
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while (n != nullptr)
    {
        if (n->msg_ == msg)
        {
            return MsgToken(n, this);
        }
        n = n->next_[level];
    }
    return end();
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken LabelList::find(size_t index, const LabelType& label)
{
    //O(|L|)
    size_t lbl_index = this->getLabelIndex(label);

    if (this->heads_.size() <= index) return end();

    MsgNode* curr = heads_[lbl_index];

    for (uint i = 0; i < index; i++)
    {
        curr = curr->next_[lbl_index];
    }

    if (curr)
    {
        return MsgToken(curr, this);
    }
    return end();
}

// Complete - Do not alter
LabelList::MsgToken const& LabelList::end() const { return end_; }

// Complete - Do not alter
void LabelList::print(std::ostream& ostr, const LabelType& label, char separator) const {
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while (nullptr != n)
    {
        ostr << n->msg_ << separator;
        n = n->next_[level];
    }
    ostr << std::endl;
}

// To be completed - must adhere to any requirements in the .h file
size_t LabelList::findLabelIndex(const LabelType& label) const {
    for (uint i = 0; i < labels_.size(); i++)
    {
        if (labels_[i] == label)
        {
            return i;
        }
    }

    return INVALID_LABEL;
}

// Complete - Do not alter
size_t LabelList::getLabelIndex(const LabelType& label) const {
    size_t retval = this->findLabelIndex(label);
    if (INVALID_LABEL == retval)
    {
        throw std::out_of_range("Label doesn't exist");
    }
    return retval;
}

bool LabelList::MsgNode::hasLabel(const int& labelIndex) const {
    return labelIndex < this->labelled_.size()
        && this->labelled_[labelIndex];
}