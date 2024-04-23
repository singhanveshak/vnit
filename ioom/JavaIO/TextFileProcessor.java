import java.io.*;


public class TextFileProcessor {
    
    // Method to read text data from an existing text file
    public static String readFile(String filename) {
        StringBuilder content = new StringBuilder();
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = reader.readLine()) != null) {
                content.append(line).append("\n");
            }
        } catch (FileNotFoundException e) {
            System.err.println("File not found: " + filename);
        } catch (IOException e) {
            System.err.println("Error reading file: " + e.getMessage());
        }
        return content.toString();
    }

    // Method to write text data to a new text file
    public static void writeFile(String filename, String content) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            writer.write(content);
        } catch (IOException e) {
            System.err.println("Error writing to file: " + e.getMessage());
        }
    }

    // Method to count the number of words in the text
    public static int countWords(String text) {
        return text.split("\\s+").length;
    }

    // Method to count the number of sentences in the text
    public static int countSentences(String text) {
        // Split by '.', '!', '?'
        String[] sentences = text.split("[.!?]+");
        return sentences.length;
    }

    // Method to find and replace specific words
    public static String findAndReplace(String text, String targetWord, String replacement) {
        return text.replaceAll("\\b" + targetWord + "\\b", replacement);
    }

    public static void main(String[] args) {
        // Test file reading
        String filename = "input.txt";
        String text = readFile(filename);
        System.out.println("Original text:\n" + text);

        // Test text processing
        int wordCount = countWords(text);
        int sentenceCount = countSentences(text);
        System.out.println("Word count: " + wordCount);
        System.out.println("Sentence count: " + sentenceCount);

        // Test find and replace
        String targetWord = "apple";
        String replacement = "banana";
        String newText = findAndReplace(text, targetWord, replacement);
        System.out.println("Text after replacing '" + targetWord + "' with '" + replacement + "':\n" + newText);

        // Test file writing
        String outputFilename = "output.txt";
        writeFile(outputFilename, newText);
        System.out.println("Text has been written to " + outputFilename);
    }
}
