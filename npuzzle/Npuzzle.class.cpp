/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Npuzzle.class.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 08:57:37 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/01 08:57:50 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "Npuzzle.class.hpp"

using namespace std;

static uint64_t getUnsigned( istream& is ) {
	int64_t	value;

	try {
		is >> value;
		if ( !is || is.fail() )
			throw runtime_error( "Non-digit character detected" );
		if ( value < 0 )
			throw range_error( "Negative number detected" );
	}
	catch ( const exception& e ) {
		cerr << endl << "Error: " << e.what() << endl;
		exit ( 1 );
	}
	return ( value );
}

Npuzzle::Npuzzle( void )
{
	uint64_t	size = 0;

	cout << "No board file was provided. Entering random mode." << endl;
	while ( size < 3 || size > 16 ) {
		cout << endl << "Enter length of board's side (3<=x<=16): ";
		size = int( getUnsigned(cin) );
	}
	solution_ = Board( size );
	solution_.generateCorrect();
	solution_.setHash();
	cout << "Generating random board of size " << size << endl;
	initial_ = Board( size );
	initial_.generateRandom();
	cout << "Initial board:\n" << initial_;
	maxOpen_ = 0;
	maxCost_ = 0;
	setMetricId();
	setSearchType();
	initial_.setHash();
	initial_.setWeight(metricId_, searchType_, solution_);
}

Npuzzle::Npuzzle( std::istream& infile ) {
	uint64_t	size;
	
	cout << "Length of board's side: ";
	while (infile.peek() == '#') {
		infile.clear();
		infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	size = getUnsigned( infile );
	cout << size << endl;
	solution_ = Board( size );
	solution_.generateCorrect();
	solution_.setHash();
	initial_ = Board( size );
	infile >> initial_;
	cout << initial_;
	maxOpen_ = 0;
	maxCost_ = 0;
	setMetricId();
	setSearchType();
	initial_.setHash();
	initial_.setWeight(metricId_, searchType_, solution_);
}

void	Npuzzle::setMetricId( void ) {
	vector<string> names = { "Manhattan", "Euclidean", "Hamming" };
	cout << "\nAvailable metrics:" << endl;
	cout << "\t0 - Manhattan (manhattan, or cab distance: dx + dy)" << endl;
	cout << "\t1 - Euclidean (euclidean distance: sqrt(dx^2 + dy^2)" << endl;
	cout << "\t2 - Hamming (number of misplaced nodes)" << endl;
	metricId_ = -1;
	while ( metricId_ < 0 || metricId_ > 2 ) {
		cout << "\nType metric id (0 or 1 or 2): ";
		metricId_ = int( getUnsigned(cin) );
	}
	cout << names[metricId_] << " metric selected" << endl;
}

void	Npuzzle::setSearchType( void ) {
	vector<string> names = { "Standard", "Greedy", "Cost-uniform" };
	cout << "\nAvailble search types:" << endl;
	cout << "\t0 - Standard     : f(g)+f(h)" << endl;
	cout << "\t1 - Greedy       : f(g)" << endl;
	cout << "\t2 - Cost-uniform : f(h)" << endl;
	searchType_ = -1;
	while ( searchType_ < 0 || searchType_ > 2 ) {
		cout << "\nType search id (0 or 1 or 2): ";
		searchType_ = int( getUnsigned(cin) );
	}
	cout << names[metricId_] << " search type selected" << endl;
}

bool	Npuzzle::solve( void ) {
	openStates_.insert( {initial_.getWeight(), &initial_} );
	allHashes_.insert( initial_.getHash() );
	while ( openStates_.size() > 0 ) {
		Board *current = openStates_.begin()->second;
		if ( current->getHash() == solution_.getHash() ) {
			return ( print(current) );
		} else {
			current->setChildren(metricId_, searchType_, solution_);
			vector<Board> *newChildren = current->getChildren();
			if ( newChildren[0][0].getCost() > maxCost_ ) {
				maxCost_ = newChildren[0][0].getCost();
			}
			for ( auto newChild : *newChildren ) {
				if ( allHashes_.count(newChild.getHash()) == 0 ) {
					allHashes_.insert(newChild.getHash());
					openStates_.insert( {newChild.getWeight(), &newChild} );
				}
			}
			openStates_.erase(openStates_.begin());
			if ( (int)openStates_.size() > maxOpen_ ) {
				maxOpen_ = (int)openStates_.size();
			}
		}
	}
	return ( false );
}

void 	recursivePrintSteps( const Board* solution, int& res ) {
	if ( solution->getParent() ) {
		res++;
		recursivePrintSteps( solution->getParent(), res );
	}
	cout << *solution << endl;
}

bool	Npuzzle::print( Board* solution ) const {
	int steps = 0;
	recursivePrintSteps( solution, steps );
	cout << "Solution steps  : " << steps << endl;
	cout << "Time complexity : " << maxCost_ << endl;
	cout << "Space complexity: " << maxOpen_ << endl;
	return ( true );
}
