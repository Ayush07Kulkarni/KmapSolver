from flask import Flask, render_template, request
import subprocess

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def index():
    output = None
    if request.method == 'POST':
        # Get form data safely
        nBits = request.form.get('nBits', '').strip()
        minTerms = request.form.get('minTerms', '').strip()
        dontCare = request.form.get('dontCare', '').strip()

        if not nBits or not minTerms:
            return render_template('index.html', output="Error: Number of variables and minterms are required.")

        # Prepare input data
        minTerms_list = minTerms.split()
        dontCare_list = dontCare.split() if dontCare else []

        input_data = f"{nBits} {len(minTerms_list)} {' '.join(minTerms_list)} {len(dontCare_list)} {' '.join(dontCare_list)}"

        # Compile and run the C++ program
        try:
            subprocess.run(['g++', 'kmap_solver.cpp', '-o', 'kmap_solver'], check=True)
            result = subprocess.run(['./kmap_solver'], input=input_data, text=True, capture_output=True, check=True)
            output = result.stdout
        except subprocess.CalledProcessError as e:
            output = f"An error occurred: {e.stderr} {e.stdout}"

    return render_template('index.html', output=output)

if __name__ == '__main__':
    app.run(debug=True)
