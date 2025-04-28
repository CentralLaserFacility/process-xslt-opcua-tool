// Author : Tim Speight - STFC  20-Mar-2025
#include <iostream>
#include <vector>
#include <unordered_map>
#include <SaxonProcessor.h>
#include <fstream>
#include <filesystem>

struct TransformationResult {
    int success = 0;
    int failure = 0;
    std::vector<std::string> failureList;
};

XsltExecutable* compileStylesheet(SaxonProcessor* processor, const std::string& xsltFile, TransformationResult& result) {
    Xslt30Processor* xsltProc = processor->newXslt30Processor();
    if (!xsltProc) {
        std::cerr << "Error: Failed to create Xslt30Processor instance." << std::endl;
        result.failure++;
        result.failureList.push_back(xsltFile);
        return nullptr;
    }

    try {
        std::cout << "Compiling XSLT file: " << xsltFile << std::endl;
        return xsltProc->compileFromFile(xsltFile.c_str());
    } catch (SaxonApiException& e) {
        std::cerr << "Error compiling stylesheet " << xsltFile << ": " << e.what() << std::endl;
        result.failure++;
        result.failureList.push_back(xsltFile);
    }
    return nullptr;
}

bool applyTransformation(XsltExecutable* executable, const std::string& xmlFilePath, const std::string& outputFile, TransformationResult& result) {
    if (!executable) {
        std::cerr << "Error: Null XsltExecutable provided for transformation." << std::endl;
        return false;
    }

    try {
        const char* resultStr = executable->transformFileToString(xmlFilePath.c_str());
        if (resultStr) {
            std::ofstream outFile(outputFile);
            if (outFile.is_open()) {
                outFile << resultStr;
                outFile.close();
                std::cout << "Transformation successful. Output saved to: " << outputFile << std::endl;
                result.success++;
                return true;
            } else {
                std::cerr << "Error: Could not open output file: " << outputFile << std::endl;
            }
        }
    } catch (SaxonApiException& e) {
        std::cerr << "Error during transformation: " << e.what() << std::endl;
    }

    result.failure++;
    result.failureList.push_back(outputFile);
    return false;
}

void processFile(const std::string& xmlFilePath, const std::string& xsltFolder, const std::string& outputDirectory, SaxonProcessor* processor, TransformationResult& result) {
    for (const auto& entry : std::filesystem::directory_iterator(xsltFolder)) {
        if (entry.path().extension() == ".xsl") {
            std::string xsltFile = entry.path().string();
            std::string outputFile = outputDirectory + "/" + entry.path().stem().string() + ".output";
            XsltExecutable* executable = compileStylesheet(processor, xsltFile, result);
            if (!applyTransformation(executable, xmlFilePath, outputFile, result)) {
                std::cerr << "Failed to transform XML file with stylesheet: " << xsltFile << std::endl;
            }
            delete executable;  // Clean up
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <xmlFilePath> <xsltFolder> <outputDirectory>" << std::endl;
        return 1;
    }

    std::string xmlFilePath = argv[1];
    std::string xsltFolder = argv[2];
    std::string outputDirectory = argv[3];
    SaxonProcessor* processor = new SaxonProcessor(true);

    TransformationResult result;
    processFile(xmlFilePath, xsltFolder, outputDirectory, processor, result);

    delete processor;

    std::cout << "\nSummary of Transformations:" << std::endl;
    std::cout << "  Successful: " << result.success << std::endl;
    std::cout << "  Failed: " << result.failure << std::endl;
    if (!result.failureList.empty()) {
        std::cout << "  Failed Files:" << std::endl;
        for (const auto& file : result.failureList) {
            std::cout << "    - " << file << std::endl;
        }
    }

    return result.failure == 0 ? 0 : 1;
}
