//
// Combat-Q, using React JS, main application hook.
//
// Copyright 2017, Peter Mikelsons
//
import update from 'immutability-helper';
import React, { Component } from 'react';
import './App.css';

class App extends Component {
    constructor(props) {
        super(props);

        this.addPerformer = this.addPerformer.bind(this);
        this.unselectPerformer = this.unselectPerformer.bind(this);
        this.handleSelectedPerformerNameInputChange = this.handleSelectedPerformerNameInputChange.bind(this);

        this.state = {
            // Counter used to generate unique id for each Performer
            nextId: 1,
            // Collection of all "Performer" instances - i.e. PCs, NPCs, monster, or spells
            performers: [],
            // Which Performer is "selected" for special editing
            selectedPerformer: undefined,
            // TODO extract Performer detail editor component and get rid of selectedPerformerName
            // selectedPerfName is used for a controlled input, which should not be null or undefined
            selectedPerformerName: ""
        };
    }
    addPerformer(e) {
        // Clicking anywhere unselects, but we want to select new Performer
        e.stopPropagation();
        var performer = {
            name: "new character",
            isActive: false,
            id: this.state.nextId
        };
        // Add new guy, select it, and update the ID counter.
        this.setState(update(this.state, {
            performers: {$push: [performer]},
            selectedPerformer: {$set: performer},
            selectedPerformerName: {$set: performer.name},
            nextId: {$apply: nextId => nextId + 1}
        }));
    }
    unselectPerformer() {
        this.setState({
            selectedPerformerName: "",
            selectedPerformer: undefined
        });
    }
    handleSelectedPerformerNameInputChange(event) {
        var name = event.target.value;
        var index = this.state.performers.indexOf(this.state.selectedPerformer);
        if (index < 0) {
            return;
        }
        var updatedPerformer = update(this.state.selectedPerformer, {
            name: {$set: name}
        })
        this.setState(update(this.state, {
            performers: {$splice: [[index, 1, updatedPerformer]]},
            selectedPerformer: {$set: updatedPerformer},
            selectedPerformerName: {$set: name}
        }));
    }
    stopEventPropagation(e) {
        e.stopPropagation();
    }
    // should be called with context=this
    getFunctionToSelectPerformer(context, performer) {
        return function(e) {
            // Clicking anywhere unselects, but we want to select performer
            e && e.stopPropagation();
            context.setState({
                selectedPerformer: performer,
                selectedPerformerName: performer.name
            });
        };
    }
    getFunctionToSetPerformerIsActive(context, performer) {
        return function(e) {
            var index = context.state.performers.indexOf(performer);
            if (index < 0) {
                return;
            }
            var updatedPerformer = update(performer, {
                isActive: {$set: event.target.value}
            })
            var change = {
                performers: {$splice: [[index, 1, updatedPerformer]]}
            };
            if (context.state.selectedPerformer === performer) {
                change.selectedPerformer = {$set: updatedPerformer};
            }
            context.setState(update(context.state, change));
        }
    }
    // should be called with context=this
    getFunctionToRemovePerformer(context, performer) {
        return function(e) {
            var index = context.state.performers.indexOf(performer);
            if (index < 0) {
                return;
            }
            context.setState(update(context.state, {
                performers: {$splice: [[index, 1]]}
            }), function() {
                // Don't leave a deleted Performer as selected.
                if (performer === context.state.selectedPerformer) {
                    context.unselectPerformer();
                }
            });
        }; 
    }
    render() {
        const performerRows = this.state.performers.map(p => (
            <tr key={p.id} onClick={this.getFunctionToSelectPerformer(this, p)}>
                <td><input type="checkbox" value={p.isActive} onChange={this.getFunctionToSetPerformerIsActive(this, p)} /></td>
                <td>{p.name}</td>
                <td><button onClick={this.getFunctionToRemovePerformer(this, p)}>Remove</button></td>
            </tr>
        ));
        return (
        <div className="App" onClick={this.unselectPerformer}>
            <div className="App-header">
                <h2>Combat-Q Overview</h2>
            </div>
            {performerRows.length > 0 && <table>
                <tbody>
                    <tr>
                        <th title="Is or should be in current combat">Active?</th>
                        <th>Name</th>
                    </tr>
                    {performerRows}
                </tbody>
            </table>}
            <button onClick={this.addPerformer}>Add</button>
            {this.state.selectedPerformer && <div>
                <span name="selName">Name:</span>
                <input type="text" autoFocus
                    value={this.state.selectedPerformerName}
                    onChange={this.handleSelectedPerformerNameInputChange}
                    // Clicking anywhere unselects, but we want to preserve current selection.
                    onClick={this.stopEventPropagation}
                    />
            </div>}
        </div>
        );
    }
}

export default App;
