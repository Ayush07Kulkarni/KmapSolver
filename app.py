from flask import Flask, render_template, request
import subprocess

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def index():
    output = None
    if request.method == 'POST':
        # Get form data
        nBits = request.form['nBits']
        inputType = request.form['inputType']
        dontCare = request.form['dontCare'].strip()

        # Prepare input data based on the selected input type
        if inputType == 'minterms':
            minTerms = request.form['minTerms'].strip()
            minTerms_list = minTerms.split()
            input_data = f"{nBits} {len(minTerms_list)} {' '.join(minTerms_list)} {len(dontCare.split())} {dontCare}"
        else:  # K-map input
            kmap = request.form['kmap'].strip()
            minTerms = convert_kmap_to_minterms(kmap)
            input_data = f"{nBits} {len(minTerms.split())} {minTerms} {len(dontCare.split())} {dontCare}"

        # Compile and run the C++ program (corrected to use kmap_solver.cpp)
        try:
            subprocess.run(['g++', 'kmap_solver.cpp', '-o', 'kmap_solver'], check=True)
            result = subprocess.run(['./kmap_solver'], input=input_data, text=True, capture_output=True, check=True)
            output = result.stdout
        except subprocess.CalledProcessError as e:
            # Capture both stdout and stderr
            output = f"An error occurred: {e.stderr} {e.stdout}"

    return render_template('index.html', output=output)

def convert_kmap_to_minterms(kmap):
    # This function should convert K-map input to minterms.
    # Example placeholder (you'll need to implement actual logic based on your K-map format):
    return "0 1 2"  # Replace with actual conversion logic

if __name__ == '__main__':
    app.run(debug=True)
