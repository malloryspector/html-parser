## HTML Parser

Final project for CS5002 Discrete and Data Structures. This project is an HTML parser, which takes in an html file and either outputs an error if tags are not being used correctly, or emulates their effect in text if tags are used correctly.

Version 1.0 supports the following tags: <div>, <h1>, <h2>, <h3>, <p>, <br>, <img>, <b>, <i>, <ul>, <li>, <link>, <meta>.

### How to Use It

This program prompts the user for the desired HTML file to parse. There are three outcomes that can occur as long as the file input is valid.

**Successful parsing** - If the file uses tags correctly, the program will then emulate the html file in the console. The parser and text emulation do take into account singleton tags (ones that don’t need a closing tag) and no attributes enclosed in a tag will be printed to the console.

    > ./parser
    Enter html file to parse: tests/success_test.html

    -----------------------------
     HTML has passed all tests!
    ----------------------------

    About — Constellation Calligraphy
    - portfolio
    - blog
    - about
    - contact


**Failure comparing tags during parsing** - If the parser encounters an error while comparing the opening tag from the stack and the current closing tag, the parsing will stop completely and an error will be printed to console.

    > ./parser
    Enter html file to parse: tests/failure_compare.html

    -------------------------
     An error has been found
    -------------------------

    A tag is missing in the html file.


**Failure after parsing** - If once the file has been totally parsed there are still opening tags left in the stack, an error will be printed to console with information about the missing closing tags.

    > ./parser
    Enter html file to parse: tests/failure_stack_remainder.html

    --------------------------------------
     The following errors have been found
    --------------------------------------

    Missing closing </li> tag.
    Missing closing </b> tag.
