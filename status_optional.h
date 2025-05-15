#include <string>
#include <optional>

/*!
 * \brief The StatusOptional class represent an optional value, with an optional message.
 *
 * StatusOptional<T, MsgT> can be constructed from a value of Type T, representing an instance of T,
 * using the special function StatusOptional<T, MsgT> StatusOptional<T, MsgT>::warning(T const& val, MsgT const& msg),
 * it is also possible to create a StatusOptional representing T, but also containing an additional message
 * giving some information about the warning. Finally, using the function
 * StatusOptional<T, MsgT> StatusOptional<T, MsgT>::error(MsgT const& msg), it is possible to return a message
 * instead of a value.
 *
 * The also exist a specialization StatusOptional<void, MsgT>, which contains no value but still
 * indicate if the results is valid (no message provided), is a warning (a message was provided, but not as an error)
 * or an error (a message has been provided and should be treated as an error).
 */
template <typename T, typename MsgT = std::string>
class StatusOptional {
public:

    typedef T ValueType;
    typedef MsgT MessageType;

    static StatusOptional<T, MsgT> warning(T const& val, MsgT const& msg) {
        StatusOptional<T, MsgT> ret;
        ret._val = val;
        ret._msg = msg;
        return ret;
    }

    static StatusOptional<T, MsgT> warning(T && val, MsgT const& msg) {
        StatusOptional<T, MsgT> ret;
        ret._val = std::move(val);
        ret._msg = msg;
        return ret;
    }

    static StatusOptional<T, MsgT> warning(T const& val, MsgT && msg) {
        StatusOptional<T, MsgT> ret;
        ret._val = val;
        ret._msg = std::move(msg);
        return ret;
    }

    static StatusOptional<T, MsgT> warning(T && val, MsgT && msg) {
        StatusOptional<T, MsgT> ret;
        ret._val = std::move(val);
        ret._msg = std::move(msg);
        return ret;
    }

    static StatusOptional<T, MsgT> error(MsgT const& msg) {
        StatusOptional<T, MsgT> ret;
        ret._val = std::nullopt;
        ret._msg = msg;
        return ret;
    }

    static StatusOptional<T, MsgT> error(MsgT && msg) {
        StatusOptional<T, MsgT> ret;
        ret._val = std::nullopt;
        ret._msg = std::move(msg);
        return ret;
    }


    StatusOptional() :
        _val(std::nullopt),
        _msg(std::nullopt)
    {

    }

    StatusOptional(T const& val) :
        _val(val),
        _msg(std::nullopt)
    {

    }

    StatusOptional(T && val) :
        _val(std::move(val)),
        _msg(std::nullopt)
    {

    }

    StatusOptional(StatusOptional<T,MsgT> const& other) :
        _val(other._val),
        _msg(other.message())
    {

    }

    StatusOptional(StatusOptional<T,MsgT> && other) :
        _val(std::move(other._val)),
        _msg(std::move(other.message()))
    {

    }

    StatusOptional<T,MsgT>& operator=(StatusOptional<T,MsgT> const& other) {
        _val = other._val;
        _msg = other._msg;
        return *this;
    }

    StatusOptional<T,MsgT>& operator=(StatusOptional<T,MsgT> && other) {
        _val = std::move(other._val);
        _msg = std::move(other._msg);
        return *this;
    }

    StatusOptional<T,MsgT>& operator=(T const& val) {
        _val = val;
        _msg = std::nullopt;
        return *this;
    }

    StatusOptional<T,MsgT>& operator=(T && val) {
        _val = std::move(val);
        _msg = std::nullopt;
        return *this;
    }

    operator bool() const{
        return _val.has_value();
    }

    inline bool has_value() const {
        return _val.has_value();
    }

    inline T& value() {
        return _val.value();
    }

    inline T const& value() const {
        return _val.value();
    }

    T* operator ->() {
        return &(_val.value());
    }

    T const* operator ->() const {
        return &(_val.value());
    }

    inline T value_or(T const& alt) {
        if (has_value()) {
            return value();
        }
        return alt;
    }

    inline bool has_message() const {
        return _msg.has_value();
    }

    inline MsgT& message() {
        return _msg.value();
    }

    inline MsgT const& message() const {
        return _msg.value();
    }


    /*!
     * \brief is_valid indicate if the StatusOptional is valid (i.e. is not default constructed)
     * \return true if the StatusOptional is valid
     */
    inline bool is_valid() const {
        return has_message() or has_value();
    }

    /*!
     * \brief is_no_error_or_warning indicate if the StatusOptional is not an error or warning
     * \return true if the StatusOptional is valid and is not an error or warning
     *
     * the StatusOptional is no error or warning if it has a value, and no message
     */
    inline bool is_no_error_or_warning() const {
        return has_value() and !has_message();
    }
    /*!
     * \brief is_clean indicate if the StatusOptional is a warning
     * \return true if the StatusOptional is a warning
     *
     * the StatusOptional is a warning if it has a value, but also a message
     * (e.g. a function could compute a result, but additional care is needed, or something needs to be logged).
     */
    inline bool is_warning() const {
        return has_value() and has_message();
    }

    /*!
     * \brief is_clean indicate if the StatusOptional is an error
     * \return true if the StatusOptional is an error
     *
     * the StatusOptional is an error if it has no value. In that case, a message has to be present.
     */
    inline bool is_error() const {
        return !has_value() and has_message();
    }

protected:
    std::optional<T> _val;
    std::optional<MsgT> _msg;
};

template <typename MsgT>
class StatusOptional<void, MsgT> {
public:

    typedef void ValueType;
    typedef MsgT MessageType;

    static StatusOptional<void, MsgT> warning(MsgT const& msg) {
        StatusOptional<void, MsgT> ret;
        ret._isValid = true;
        ret._msg = msg;
        return ret;
    }

    static StatusOptional<void, MsgT> warning(MsgT && msg) {
        StatusOptional<void, MsgT> ret;
        ret._isValid = true;
        ret._msg = std::move(msg);
        return ret;
    }

    static StatusOptional<void, MsgT> error(MsgT const& msg) {
        StatusOptional<void, MsgT> ret;
        ret._isValid = false;
        ret._msg = msg;
        return ret;
    }


    static StatusOptional<void, MsgT> error(MsgT && msg) {
        StatusOptional<void, MsgT> ret;
        ret._isValid = false;
        ret._msg = std::move(msg);
        return ret;
    }


    StatusOptional() :
        _isValid(true),
        _msg(std::nullopt)
    {

    }

    StatusOptional(StatusOptional<void,MsgT> const& other) :
        _isValid(other._isValid),
        _msg(other.message())
    {

    }

    StatusOptional(StatusOptional<void,MsgT> && other) :
        _isValid(other._isValid),
        _msg(std::move(other.message()))
    {

    }

    StatusOptional<void,MsgT>& operator=(StatusOptional<void,MsgT> const& other) {
        _isValid = other._isValid;
        _msg = other._msg;
        return *this;
    }

    StatusOptional<void,MsgT>& operator=(StatusOptional<void,MsgT> && other) {
        _isValid = other._isValid;
        _msg = std::move(other._msg);
        return *this;
    }

    operator bool() const{
        return _isValid;
    }

    inline bool has_message() const {
        return _msg.has_value();
    }

    inline MsgT& message() {
        return _msg.value();
    }

    inline MsgT const& message() const {
        return _msg.value();
    }


    /*!
     * \brief is_valid indicate if the StatusOptional is valid (i.e. is not default constructed)
     * \return true if the StatusOptional is valid
     */
    inline bool is_valid() const {
        return has_message() or _isValid;
    }

    /*!
     * \brief is_no_error_or_warning indicate if the StatusOptional is not an error or warning
     * \return true if the StatusOptional is valid and is not an error or warning
     *
     * the StatusOptional is no error or warning if it has a value, and no message
     */
    inline bool is_no_error_or_warning() const {
        return _isValid and !has_message();
    }
    /*!
     * \brief is_clean indicate if the StatusOptional is a warning
     * \return true if the StatusOptional is a warning
     *
     * the StatusOptional is a warning if it has a value, but also a message
     * (e.g. a function could compute a result, but additional care is needed, or something needs to be logged).
     */
    inline bool is_warning() const {
        return _isValid and has_message();
    }

    /*!
     * \brief is_clean indicate if the StatusOptional is an error
     * \return true if the StatusOptional is an error
     *
     * the StatusOptional is an error if it has no value. In that case, a message has to be present.
     */
    inline bool is_error() const {
        return !_isValid and has_message();
    }

protected:
    bool _isValid;
    std::optional<MsgT> _msg;
};
