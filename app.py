from flask import Flask, render_template, request
import subprocess
from compliment import compliment_string
app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def index():
    output = None
    if request.method == 'POST':
        # Get form data
        isSop = request.form["Select Expression Type"]
        nBits = request.form['nBits']
        dontCare = request.form['dontCare'].strip()

        # Prepare input data based on the selected input type
       
    minTerms = request.form['minTerms'].strip()
    minTerms_list = minTerms.split()
    input_data = f"{nBits} {len(minTerms_list)} {' '.join(minTerms_list)} {len(dontCare.split())} {dontCare}"
        

        # Compile and run the C++ program (corrected to use kmap_solver.cpp)
    try:
        subprocess.run(['g++', 'kmap_solver.cpp', '-o', 'kmap_solver'], check=True)
        result = subprocess.run(['./kmap_solver'], input=input_data, text=True, capture_output=True, check=True)
    
        if isSop == "SOP":
            output = result.stdout
        else:
            output = compliment_string(result.stdout)
    except subprocess.CalledProcessError as e:
            # Capture both stdout and stderr
        output = f"An error occurred: {e.stderr} {e.stdout}"

    return render_template('index.html', output=output)


if __name__ == '__main__':
    app.run(debug=True)
