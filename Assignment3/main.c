#include <assert.h>
#include "document_analyzer.h"

int main(void)
{
    const char*** paragraph;
    const char** sentence;
    size_t y;
    load_document("/Users/sejun/Downloads/Assignment3_Resources/input.txt");
    y = get_total_word_count(); /* 62 */
    y = get_total_sentence_count(); /* 6 */
    y = get_total_paragraph_count(); /* 3 */
    paragraph = get_paragraph(2); /* [ [ "Egestas", "diam", "in", "arcu", "cursus", "euismod", "quis", "viverra" ], [ "Nunc", "non", "blandit", "massa", "enim", "nec", "dui" ] ] */


    paragraph = get_paragraph(2);
    y = get_paragraph_word_count(paragraph); /* 15 */
    y = get_paragraph_sentence_count(paragraph); /* 2 */

    sentence = get_sentence(1, 1); /* [ "Nisl", "purus", "in", "mollis", "nunc", "sed", "id", "semper", "risus" ] */
    y = get_sentence_word_count(sentence); /* 9 */

    print_as_tree("/Users/sejun/Downloads/Assignment3_Resources/out.txt");



    dispose();


    return 0;
}
