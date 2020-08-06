/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 08:57:29 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/06 18:32:20 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
# include "Npuzzle.class.hpp"

using namespace std;
//
int		main(int ac, char **av) {
	if (ac > 2) {
		cout << "Usage: ./npuzzle [filename]" << endl;
	}
	if (ac == 2) {
		ifstream infile;
		try {
			infile.open(av[1], ios::in);
			if (!infile || infile.fail())
				throw runtime_error("File couldn't be open");
			Npuzzle puzzle(infile);
			infile.close();
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
