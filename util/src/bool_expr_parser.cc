#include <bool_expr_parser.h>

bool BooleanExpressionParser::Parse() {
  bool result = ParseExpr();
  if (has_error_ || currentIndex_ != expression_.size()) {
    ReportError("Unexpected tokens after parsing.");
    return false; // Indicate parse error
  }
  return result;
}

bool BooleanExpressionParser::HasError() const {
  return has_error_;
}

char BooleanExpressionParser::CurrentChar() const {
  if (currentIndex_ < expression_.size()) {
    return expression_[currentIndex_];
  }
  return '\0';
}

// Advance to the next character
void BooleanExpressionParser::Consume() {
  if (currentIndex_ < expression_.size()) {
    ++currentIndex_;
  }
}

// Parse OR ('+') expressions
bool BooleanExpressionParser::ParseExpr() {
  bool result = ParseTerm(); // Parse AND expressions first
  while (CurrentChar() == '+') {
    Consume(); // Consume the '+'
    result = ParseTerm() || result;
    if (has_error_) return false; // Halt on error
  }
  return result;
}

// Parse AND ('*') expressions
bool BooleanExpressionParser::ParseTerm() {
  bool result = ParseFactor(); // Parse primary values first
  while (CurrentChar() == '*') {
    Consume(); // Consume the '*'
    result = ParseFactor() && result;
    if (has_error_) return false; // Halt on error
  }
  return result;
}

// Parse primary values: variables (a-z), optionally negated with `'`
bool BooleanExpressionParser::ParseFactor() {
  char token = CurrentChar();
  if (std::isalpha(token)) {
    Consume(); // Consume the letter (e.g., 'a')
    bool value;
    if (values_.find(token) != values_.end()) {
      value = values_.at(token);
    } else {
      ReportError("Undefined variable: " + std::string(1, token));
      return false;
    }

    // Check for negation (look-ahead for `'`)
    if (CurrentChar() == '\'') {
      Consume(); // Consume the `'`
      value = !value; // Apply negation
    }

    return value;
  } else {
    ReportError("Unexpected token: " + std::string(1, token));
    return false;
  }
}

void BooleanExpressionParser::ReportError(const std::string& message) {
  if (!has_error_) {
    err_msg_ = "Error: " + message;
    has_error_ = true;
  }
}

const std::string BooleanExpressionParser::Error() const {
  return err_msg_;
}


const std::string Explode(const char input[], char delim) {
  std::string result;

  for (const char* c = input; *c != '\0'; ++c)
    if (*c != delim)
      result += *c;

  return result;
}


const std::unordered_map<char, bool> BuildMap(const std::string& b_vals) {
  // Read variable values
  std::unordered_map<char, bool> values;
  std::size_t varIndex = 0;
  for (char b_val : b_vals) {
    char var = 'a' + varIndex; // Variables are a, b, c, ...
    values[var] = (b_val == 'T');
    varIndex++;
  }
  return values;
}
