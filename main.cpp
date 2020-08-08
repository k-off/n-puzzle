/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 08:57:29 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/01 08:57:30 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"
# include "Npuzzle.class.hpp"

using namespace std;

int		main(int argCount, char **argValues) {
	if (argCount > 2) {
		cout << "Usage: ./npuzzle [filename]" << endl;
	}
	if (argCount == 2) {
		ifstream inputFileStream;
		try {
			inputFileStream.open(argValues[1], ios::in);
			if (!inputFileStream || inputFileStream.fail())
				throw runtime_error("File couldn't be open");
			Npuzzle puzzle(inputFileStream);
			inputFileStream.close();
			puzzle.solve();
		}
		catch ( const std::exception& e ) {
			cout << endl << "Error: " << e.what() << endl;
			exit(1);
		}
	} else {
		try {
			Npuzzle puzzle;
			puzzle.solve();
		}
		catch ( const std::exception& e ) {
			cout << endl << "Error: " << e.what() << endl;
			exit(1);
		}
	}
	return (0);
}
