#ifndef IMS_PRINT_H
#define IMS_PRINT_H

#include <iostream>
#include <string>

namespace ims {
	
/**
 * Class contains various static utility methods to print data structures to standard output.
 */
class PrintingUtils {
	public:
		/**
		 * Prints one dimensional container to standard output.
		 * 
		 * @param coll One dimensional container to be printed out.
		 */ 
		template <class T>
		static void printOneDimensionalContainer(const T& coll);

		/**
		 * Prints one dimensional container with preceding title to standard output.
		 * 
		 * @param coll One dimensional container to be printed out.
		 * @param optstr String to be output before container. 
		 */ 
		template <class T>
		static void printOneDimensionalContainer(const T& coll, const std::string& optstr);

		/**
		 * Prints one dimensional container of pairs to standard output.
		 * 
		 * @param coll One dimensional container of pairs to be printed out.
		 */ 
		template <class T>
		static void printOneDimensionalContainerOfPairs(const T& coll);		
		
		/**
		 * Prints one dimensional container of pairs with preceding title to standard output.
		 * 
		 * @param coll One dimensional container of pairs to be printed out.
		 * @param optstr String to be output before container.
		 */ 
		template <class T>
		static void printOneDimensionalContainerOfPairs(const T& coll, const std::string& optstr);
		
		/**
		 * Prints one dimensional container of pairs with preceding title to standard output.
		 * NOTE: The output doesn't begin a new line after printing out the container.
		 * 
		 * @param coll One dimensional container of pairs to be printed out.
		 * @param optstr String to be output before container.
		 * @param delimiter Delimiter between elements of one pair.
		 * @param openSymbol String, that precedes the pair output
		 * @param closeSymbol String, that finishs the pair output
		 * 
		 * @see printlnOneDimensionalContainerOfPairs
		 */ 
		template <class T>
		static void printOneDimensionalContainerOfPairs(const T& coll, const std::string& optstr, 
					const std::string& delimiter, const std::string& openSymbol, const std::string& closeSymbol);
		
		/**
		 * Prints one dimensional container of pairs with preceding title to standard output.
 		 * NOTE: The output begins a new line after printing out the container.
 		 * 
		 * @param coll One dimensional container of pairs to be printed out.
		 * @param optstr String to be output before container.
		 * @param delimiter Delimiter between elements of one pair.
		 * @param openSymbol String, that precedes the pair output
		 * @param closeSymbol String, that finishs the pair output
		 * 
		 * @see printOneDimensionalContainerOfPairs
		 */ 
		template <class T>
		static void printlnOneDimensionalContainerOfPairs(const T& coll, const std::string& optstr, 
					const std::string& delimiter, const std::string& openSymbol, const std::string& closeSymbol);
		/**
		 * Prints two dimensional container to standard output.
		 * 
		 * @param coll Two dimensional container to be printed out.
		 */ 				
		template <class T>
		static void printTwoDimensionalContainer(const T& coll);
		
		/**
		 * Prints two dimensional container with preceding title to standard output.
		 * 
		 * @param coll Two dimensional container to be printed out.
		 * @param optstr String to be output before container 
		 */ 		
		template <class T>
		static void printTwoDimensionalContainer(const T& coll, const std::string& optstr);	
		
		/**
		 * P
		 */
		template <class T>
		static void printCompomer(T* compomer);
		
};

template <class T>
void PrintingUtils::printOneDimensionalContainer(const T& coll){
	PrintingUtils::printOneDimensionalContainer(coll, "");
}

template <class T>
void PrintingUtils::printOneDimensionalContainer(const T& coll, const std::string& optstr) {
	typename T::const_iterator pos;
	if (!optstr.empty()) {
		std::cout << optstr << std::endl;
	}
	std::cout << '[';
	for(pos = coll.begin(); pos != coll.end()-1; ++pos) {
		std::cout << *pos << ' ';
	}
	std::cout << *pos << ']'; 
}

template <class T>
void PrintingUtils::printOneDimensionalContainerOfPairs(const T& coll) {
	PrintingUtils::printOneDimensionalContainerOfPairs(coll, "");
}

template <class T>
void PrintingUtils::printOneDimensionalContainerOfPairs(const T& coll, const std::string& optstr) {
	PrintingUtils::printOneDimensionalContainerOfPairs(coll, "", ",", "(", ")");
}

template <class T>
void PrintingUtils::printOneDimensionalContainerOfPairs(const T& coll, const std::string& optstr, 
							const std::string& delimiter, const std::string& openSymbol, const std::string& closeSymbol) {
	if (!optstr.empty()) {
		std::cout << optstr << std::endl;
	}
	std::cout << '[';
	typename T::size_type size = coll.size();
	for(typename T::const_iterator pos = coll.begin(), end = coll.end(); pos != end; ++pos) {
		std::cout << openSymbol << pos->first << delimiter << pos->second << closeSymbol; 
		if (--size > 0) {
			std::cout << ' ';
		}
	}
	std::cout << ']';
	
}	

template <class T>
void PrintingUtils::printlnOneDimensionalContainerOfPairs(const T& coll, const std::string& optstr, 
							const std::string& delimiter, const std::string& openSymbol, const std::string& closeSymbol) {
	PrintingUtils::printOneDimensionalContainerOfPairs(coll, optstr, delimiter, openSymbol, closeSymbol);
	std::cout << std::endl;
}	

template <class T>
void PrintingUtils::printTwoDimensionalContainer(const T& coll) {
	PrintingUtils::printTwoDimensionalContainer(coll, "");
}

template <class T>
void PrintingUtils::printTwoDimensionalContainer(const T& coll, const std::string& optstr) {
	typename T::const_iterator pos;
	if (!optstr.empty()) {
		std::cout << optstr << std::endl;
	}
	std::cout << '[';
	for(pos = coll.begin(); pos != coll.end(); ++pos) {
		printOneDimensionalContainer(*pos);
	}
	std::cout << ']' << std::endl;
}
	
template <class T>
void PrintingUtils::printCompomer(T* compomer) {
	PrintingUtils::printOneDimensionalContainerOfPairs(*compomer, "", "", "", "");
}

} // namespace ims

#endif //IMS_PRINT_H
