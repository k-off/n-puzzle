/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Npuzzle.class.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/01 08:57:37 by pacovali      #+#    #+#                 */
/*   Updated: 2020/08/06 18:32:27 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "Npuzzle.class.hpp"

using namespace std;

static int getUnsigned( istream& is ) {
	int		size {};

	try {
		is >> size;
		if ( !is || is.fail() )
			throw runtime_error( "Non-digit character detected" );
		if ( size < 0 )
			throw range_error( "Negative number detected" );
	}
	catch ( const exception& e ) {
		cerr << endl << "Error: " << e.what() << endl;
		exit ( 1 );
	}
	return ( size );
}

Npuzzle::Npuzzle( void )
{
	int		size;

	cout << "No board file was provided. Entering random mode." << endl;
	cout << endl << "Enter length of board's side: ";
	size = getUnsigned( cin );
	solution_ = Board( size );
	solution_.generateCorrect();
	cout << "Generating random board of size " << size << endl;
	initial_ = Board( size );
	initial_.generateRandom();
	cout << "Initial board:\n" << initial_;
	maxStates_ = 0;
	shortestPath_ = 0;
	setMetricId();
	initial_.setDistance(metricId_, solution_);
}

Npuzzle::Npuzzle( std::istream& infile )
: maxStates_(0)
, shortestPath_(0)
{
	int		size;
	
	cout << "Length of board's side: ";
	while (infile.peek() == '#') {
		infile.clear();
		infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	size = getUnsigned( infile );
	cout << size << endl;
	solution_ = Board( size );
	solution_.generateCorrect();
	initial_ = Board( size );
	infile >> initial_;
	cout << initial_;
	maxStates_ = 0;
	shortestPath_ = 0;
	setMetricId();
	initial_.setDistance(metricId_, solution_);
}

void	Npuzzle::setMetricId( void ) {
	vector<string> names = { "Manhattan", "Euclidean", "Hamming" };
	cout << "Available metrics:" << endl;
	cout << "\t0 - Manhattan (manhattan, or cab distance: dx + dy)" << endl;
	cout << "\t1 - Euclidean (euclidean distance: sqrt(dx^2 + dy^2)" << endl;
	cout << "\t2 - Hamming (number of misplaced nodes)" << endl;
	metricId_ = -1;
	while ( metricId_ < 0 || metricId_ > 2 ) {
		cout << "Type metric id (0 or 1 or 2): ";
		metricId_ = getUnsigned( cin );
	}
	cout << names[metricId_] << " metric selected" << endl;
}

bool	Npuzzle::solve( void ) {
	open_.insert( {initial_.getDistance() + initial_.getCost(), &initial_} );
	while ( open_.size() > 0 ) {
		Board *current = open_.begin()->second;
		if (*current != solution_) {
			if ( maxStates_ < int(open_.size()) ) {
				maxStates_ = int(open_.size());
			}
			current->setChildren(metricId_, solution_, current);
			states_.insert(current->getValueByAddress());
			open_.erase( {current->getDistance() + current->getCost(), current} );
			vector<Board>& children = current->getChildren();
			for (size_t i = 0; i < children.size(); i++) {
				if ( !states_.count(children[i].getValueByAddress()) ) {
					open_.insert( {children[i].getDistance() + children[i].getCost(), &children[i]});
				}
			}
		} else {
			return ( print(current) );
		}
	}
	return ( false );
}

int 		recursiveSteps( const Board* solution ) {
	int res = 0;

	if ( solution->getParent() ) {
		res = recursiveSteps( solution->getParent() ) + 1;
	}
	cout << *solution << endl;
	return ( res );
}

bool	Npuzzle::print( Board* solution ) const {
	int steps = recursiveSteps( solution );
	cout << "Solution steps  : " << steps << endl;
	cout << "Time complexity : " << states_.size() << endl;
	cout << "Space complexity: " << maxStates_ << endl;
	return ( true );
}
