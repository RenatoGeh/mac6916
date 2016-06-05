#include <cstdio>

#include <vector>

#include "nbayes.h"

int main(int argc, char *args[]) {
  if (argc < 2) {
    fputs("NaiveBayes classifier.\n"
        "  Usage: ./run filename\n", stdout);
    return 1;
  }

  /* Example: input/spam-train.in */
  NaiveBayes nb = NaiveBayes::Learn(args[1], 11);
  nb.WriteToFile("output/model.fg");
  nb.DrawGraph("output/graph.dot");
  fputs("\n=========================\n\n", stdout);

  /* Example:
   *  C0NGR4TUL4T10NS!!! YOU HAVE GAINED US$ 500 WORTH OF FREE MCDONALDS NUGGETS!!!
   * Result: spam
   */
  nb.Classify(nb.GetVars({1, 2, 5, 11}), {1, 1, 1, 1});
  fputs("\n=========================\n\n", stdout);

  /* Example:
   *  I just had sex with Donald Trump.
   * Result: ham
   */
  nb.Classify(nb.GetVars({3, 6}), {1, 1});
  fputs("\n=========================\n\n", stdout);

  /* Example:
   *  Enhance your performance! Buy our pills now and receive a special discount of US$ 50,00!
   * Result: spam
   */
  nb.Classify(nb.GetVars({2, 7, 8, 9, 10}), {1, 1, 1, 1, 1});
  fputs("\n=========================\n\n", stdout);

  /* Example:
   *  Dear Mr. Ham, This is an automatic receipt regarding your US$500,00 purchase of a Spamsung
   *  LCD 50" TV. Don't forget to register and become a member of BestSpam. Win special discounts
   *  and free prizes! Join now!
   * Result: ham
   */
  nb.Classify(nb.GetVars({2, 5, 8, 9}), {1, 1, 1, 1});
  fputs("\n=========================\n\n", stdout);

  return 0;
}
