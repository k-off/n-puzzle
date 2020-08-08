/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   header.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 09:23:09 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/01 09:23:54 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HEADER_H
# define __HEADER_H

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
	HAMMING			// number of misplaced nodes
};

enum SEARCH_TYPE {
	STANDARD,		// f(g) + f(h)
	GREEDY,			// f(g)
	COST_UNIFORM	// f(h)
};

#endif
