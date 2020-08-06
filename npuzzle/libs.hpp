/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libs.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 09:23:09 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/06 18:32:16 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LIBS_H
# define __LIBS_H

# include <set>
# include <map>
# include <ctime>
# include <vector>
# include <random>
# include <utility>
# include <fstream>
# include <sstream>
# include <iomanip>
# include <cstdlib>
# include <iostream>
# include <iterator>
# include <algorithm>
# include <cstdlib>

enum HEURISTICS {
	MANHATTAN,		// dx + dy
	EUCLIDEAN,		// distance a = sqrt(dx^2 + dy^2)
//	GASCHING,		// number of moves, if only swap nodes with the blank zone 		NOT IMPLEMENTED
	HAMMING,		// number of misplaced nodes
//	CONFLICT,		// if node x and y are on the same line, and x is to right of node y and should be to the left, they are in conflict (x.x_pos - y.x_pos) * 2 + 2 		NOT IMPLEMENTED
};

#endif
