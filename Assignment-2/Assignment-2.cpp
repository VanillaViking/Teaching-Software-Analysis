//===- Software-Analysis-Teaching Assignment 2-------------------------------------//
//
//     SVF: Static Value-Flow Analysis Framework for Source Code
//
// Copyright (C) <2013->
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===-----------------------------------------------------------------------===//
/*
 // Software-Analysis-Teaching Assignment 2 : Source Sink ICFG DFS Traversal
 //
 // 
 */

#include <Graphs/ICFGNode.h>
#include <Util/Casting.h>
#include <set>
#include "Assignment-2.h"
#include <iostream>
using namespace SVF;
using namespace std;

/// TODO: print each path once this method is called, and
/// add each path as a string into std::set<std::string> paths
/// Print the path in the format "START->1->2->4->5->END", where -> indicate an ICFGEdge connects two ICFGNode IDs

void ICFGTraversal::collectICFGPath(std::vector<unsigned> &path){
    string path_str = "";
    path_str.append("START->") ;

    for (int node: path) {
        path_str.append(to_string(node));
        path_str.append("->");
    }

    path_str.append("END");
    cout << path_str << endl;
    paths.insert(path_str);
}


/// TODO: Implement your context-sensitive ICFG traversal here to traverse each program path (once for any loop) from src to dst
void ICFGTraversal::reachability(const ICFGNode *src, const ICFGNode *dst)
{
    std::pair<const ICFGNode*, CallStack> pair (src, callstack);
    if (visited.count(pair) != 0) {
        return;
    }

    visited.insert(pair);
    path.push_back(src->getId());
    if (src->getId() == dst->getId()) {
        collectICFGPath(path);
    }

    for (const ICFGEdge* edge: src->getOutEdges()) {
        if (edge->isIntraCFGEdge()) {
            reachability(edge->getDstNode(), dst);
        } else if (edge->isCallCFGEdge()) {
            callstack.push_back(edge->getSrcNode());
            reachability(edge->getDstNode(), dst);
            callstack.pop_back();
        } else if (RetICFGNode* retNode = SVFUtil::dyn_cast<RetICFGNode>(edge->getDstNode())) {
            if (callstack.empty()) {
                reachability(edge->getDstNode(), dst);
            } else {
                if (callstack.back() == retNode->getCallICFGNode()) {
                    callstack.pop_back();
                    reachability(edge->getDstNode(), dst);
                    callstack.push_back(retNode->getCallICFGNode());
                }
            }
        }
    }

    visited.erase(pair);
    path.pop_back();

}
