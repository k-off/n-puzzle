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
	solutionBoard_ = Board( size );
	solutionBoard_.generateCorrect();
	cout << "Generating random board of size " << size << endl;
	initialBoard_ = Board( size );
	initialBoard_.generateRandom();
	cout << "Initial board:\n" << initialBoard_;
	maxOpenStates_ = 0;
	iterationCounter_ = 0;
	setMetricId();
	setSearchType();
	initialBoard_.setWeight(metricId_, searchTypeID_, solutionBoard_);
	initialBoard_.setHash();
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
	solutionBoard_ = Board( size );
	solutionBoard_.generateCorrect();
	initialBoard_ = Board( size );
	infile >> initialBoard_;
	cout << initialBoard_;
	maxOpenStates_ = 0;
	iterationCounter_ = 0;
	setMetricId();
	setSearchType();
	initialBoard_.setWeight(metricId_, searchTypeID_, solutionBoard_);
	initialBoard_.setHash();
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
	searchTypeID_ = -1;
	while ( searchTypeID_ < 0 || searchTypeID_ > 2 ) {
		cout << "\nType search id (0 or 1 or 2): ";
		searchTypeID_ = int( getUnsigned(cin) );
	}
	cout << names[searchTypeID_] << " search type selected" << endl;
}

bool	Npuzzle::solve( void ) {
	openStates_.insert( {initialBoard_.getWeight(), &initialBoard_} );
	allHashes_.insert( initialBoard_.getHash() );
	while ( openStates_.size() > 0 ) {
		auto first = openStates_.begin();
		Board *current = first->second;
		if ( current->getHash() == solutionBoard_.getHash() ) {
			return ( print(current) );
		} else {
			current->setChildren( metricId_, searchTypeID_, solutionBoard_ );
			for ( auto& child : current->getChildren() ) {
				if ( allHashes_.count(child.getHash()) < 1 ) {
					openStates_.insert( {child.getWeight(), &child} );
					allHashes_.insert( child.getHash() );
				}
			}
		}
		iterationCounter_++;
		openStates_.erase(first);
		if ((int)openStates_.size() > maxOpenStates_ )
			maxOpenStates_ = (int)openStates_.size();
	}
	cout << "Not solvable" << endl;
	return (false);
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
	cout << "Time complexity : " << iterationCounter_ << endl;
	cout << "Space complexity: " << maxOpenStates_ << endl;
	return ( true );
}
