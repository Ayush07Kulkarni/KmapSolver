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

string = "a'b + abc'def"
print(compliment_string(string))