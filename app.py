from flask import Flask, render_template, request
import subprocess
import os

app = Flask(__name__)

def minimize_boolean_function(nBits, mode, terms, dontCare):
    if mode == 'sop':
        # C++ code execution for SOP
        minterms = [int(term) for term in terms.split()] if terms else []
        dontcare_terms = [int(term) for term in dontCare.split()] if dontCare else []

        input_str = f"{nBits} {len(minterms)} {' '.join(map(str, minterms))} {len(dontcare_terms)} {' '.join(map(str, dontcare_terms))}"

        process = subprocess.Popen(['./kmap_solver.exe'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        stdout, stderr = process.communicate(input_str)

        if stderr:
            print(f"Error from C++ code: {stderr}")
            return {'kmap_rows': [], 'function_output': [f"Error during SOP minimization: {stderr}"]}

        output_lines = stdout.strip().split('\n')
        kmap_rows = []
        function_output = []
        kmap_start = False
        function_start = False

        for line in output_lines:
            if "0 " in line or "1 " in line:
                kmap_start = True
                kmap_rows.append([int(x) for x in line.split()])
            elif kmap_start:
                kmap_start = False
                function_start = True
            elif "Function 1:" in line:
                function_start = True
                function_output.append(line)
            elif function_start and line:
                function_output.append(line)

        return {'kmap_rows': kmap_rows, 'function_output': function_output}

    elif mode == 'pos':
        # Python compliment string function for POS
        terms = terms.replace(",", "+")
        result = compliment_string(terms)
        return {'kmap_rows': [], 'function_output': [result]}

def compliment_string(s):
    elements = s.split("+")
    complemented_terms = []

    for element in elements:
        literals = element.strip().split()
        complemented_literals = []

        for literal in element.strip():
            if literal == "'":
                continue
            elif literal.isalpha():
                if literal + "'" in element:
                    complemented_literals.append(literal)
                else:
                    complemented_literals.append(literal + "'")

        complemented_terms.append('+'.join(complemented_literals))

    return ' * '.join(complemented_terms)

@app.route('/', methods=['GET', 'POST'])
def index():
    output = None
    if request.method == 'POST':
        nBits = int(request.form['nBits'])
        mode = request.form['mode']
        terms = request.form['terms']
        dontCare = request.form['dontCare']

        output = minimize_boolean_function(nBits, mode, terms, dontCare)

    return render_template('index.html', output=output)

if __name__ == '__main__':
    app.run(debug=True)