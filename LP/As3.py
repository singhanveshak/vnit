'''
CONVERT C CODE TO 3 WORD CODE
'''

import re

class MiniCCompiler:
    def __init__(self):
        self.macros = {}
        self.variables = {}
        self.temp_var_count = 0
        self.code = []

    def preprocess(self, code):
        lines = code.split('\n')
        processed_lines = []

        for line in lines:
            line = self.remove_comments(line)
            line = self.handle_macros(line)
            if line.strip():
                processed_lines.append(line)

        return '\n'.join(processed_lines)

    def remove_comments(self, line):
        line = re.sub(r'//.*', '', line)
        line = re.sub(r'/\*.*?\*/', '', line, flags=re.DOTALL)
        return line

    def handle_macros(self, line):
        match = re.match(r'#define\s+(\w+)\s+(.+)', line)
        if match:
            self.macros[match.group(1)] = match.group(2)
            return ''
        else:
            for macro, value in self.macros.items():
                line = re.sub(r'\b' + macro + r'\b', value, line)
            return line

    def generate_temp_var(self):
        self.temp_var_count += 1
        return f't{self.temp_var_count}'

    def generate_arithmetic_code(self, expression):
        tokens = re.findall(r'(\w+|[-+*/()])', expression)
        stack = []
        operators = []
        output = []

        precedence = {'+': 1, '-': 1, '*': 2, '/': 2}

        for token in tokens:
            if re.match(r'\w+', token):
                output.append(token)
            elif token == '(':
                operators.append(token)
            elif token == ')':
                while operators and operators[-1] != '(':
                    output.append(operators.pop())
                operators.pop()
            elif token in '+-*/':
                while operators and operators[-1] != '(' and precedence.get(operators[-1], 0) >= precedence[token]:
                    output.append(operators.pop())
                operators.append(token)

        while operators:
            output.append(operators.pop())

        stack = []
        for token in output:
            if re.match(r'\w+', token):
                stack.append(token)
            else:
                operand2 = stack.pop()
                operand1 = stack.pop()
                temp_var = self.generate_temp_var()
                self.code.append(f'{temp_var} = {operand1} {token} {operand2}')
                stack.append(temp_var)

        return stack[0]

    def generate_code(self, code):
        preprocessed_code = self.preprocess(code)
        lines = preprocessed_code.split('\n')

        for line in lines:
            line = line.strip()
            if not line:
                continue

            if 'if' in line:
                self.generate_if_else_code(line)
            elif '=' in line:
                self.generate_assignment_code(line)

    def generate_assignment_code(self, line):
        parts = line.split('=')
        variable = parts[0].strip()
        expression = parts[1].strip().rstrip(';')
        result = self.generate_arithmetic_code(expression)
        self.code.append(f'{variable} = {result}')

    def generate_if_else_code(self, line):
        condition_match = re.search(r'if\s*\((.*?)\)\s*{(.*?)}', line)
        else_match = re.search(r'else\s*{(.*?)}', line)

        if condition_match: #added if condition_match
            condition = condition_match.group(1).strip()
            if_body = condition_match.group(2).strip()
            else_body = else_match.group(1).strip() if else_match else None

            condition_result = self.generate_arithmetic_code(condition)
            label_if = f'if_true_{len(self.code)}'
            label_else = f'else_{len(self.code)}'
            label_end = f'end_if_{len(self.code)}'

            self.code.append(f'if {condition_result} goto {label_if}')
            if else_body:
                self.code.append(f'goto {label_else}')
            else:
                self.code.append(f'goto {label_end}')

            self.code.append(f'{label_if}:')
            if_lines = if_body.split(';')
            for if_line in if_lines:
                if_line = if_line.strip()
                if if_line:
                    self.generate_assignment_code(if_line + ';')

            if else_body:
                self.code.append(f'goto {label_end}')
                self.code.append(f'{label_else}:')
                else_lines = else_body.split(';')
                for else_line in else_lines:
                    else_line = else_line.strip()
                    if else_line:
                        self.generate_assignment_code(else_line + ';')

            self.code.append(f'{label_end}:')

    def compile(self, code):
        self.generate_code(code)
        return '\n'.join(self.code)

# Example Usage
mini_c_code = """
#define SIZE 10
/* Multi line comment
   Test code */
int a;
int b;
int c;
a = 5 + SIZE; // single line comment
b = (a * 2) - 3;
if (b > 10) {
    c = a + b;
} else {
    c = a - b;
}
"""

compiler = MiniCCompiler()
three_address_code = compiler.compile(mini_c_code)
print(three_address_code)
