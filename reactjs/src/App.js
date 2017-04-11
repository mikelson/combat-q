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

        var methodsToBind = [
            'addPerformer',
            'unselectPerformer',
            'handleSelectedPerformerNameInputChange',
            'getPerformersInCombat',
            'isActive',
            'startCombat',
            'stopCombat',
        ];
        for (var index in methodsToBind) {
            var method = methodsToBind[index];
            this[method] = this[method].bind(this);
        }
        // Try to read previous state from local storage
        var state = localStorage.getItem("state");
        if (state) {
            try {
                state = JSON.parse(state);
            } catch (e) {
                console.error(e);
                console.log(state);
                state = undefined;
            }
        }
        this.state = state || {
            // Serialization version, for detecting backwards incompatibilities
            version: 1,
            // General state of combat: null - not in combat, 0 - getting initial rolls, 1+ running combat
            round: null,
            // Counter used to generate unique id for each Performer
            nextId: 1,
            // Collection of all "Performer" instances - i.e. PCs, NPCs, monster, or spells
            performers: [],
            // Which Performer is "selected" for special editing
            selectedPerformerId: undefined,
            // TODO extract Performer detail editor component and get rid of selectedPerformerName
            // selectedPerfName is used for a controlled input, which should not be null or undefined
            selectedPerformerName: ""
        };
    }
    setState(nextState, callback) {
        super.setState(nextState, function() {
            callback && callback();
            try {
                var json = JSON.stringify(this.state);
                console.log(json);
                localStorage.setItem("state", json);
            } catch (e) {
                console.error(e);
            }
        });
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
            selectedPerformerId: {$set: performer.id},
            selectedPerformerName: {$set: performer.name},
            nextId: {$apply: nextId => nextId + 1}
        }));
    }
    unselectPerformer() {
        this.setState({
            selectedPerformerName: "",
            selectedPerformerId: undefined
        });
    }
    handleSelectedPerformerNameInputChange(e) {
        var index = this.state.performers.findIndex(p => p.id === this.state.selectedPerformerId);
        if (index < 0) {
            return;
        }
        var name = e.target.value;
        var updatedPerformer = update(this.state.performers[index], {
            name: {$set: name}
        })
        this.setState(update(this.state, {
            performers: {$splice: [[index, 1, updatedPerformer]]},
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
                selectedPerformerId: performer.id,
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
                isActive: {$set: e.target.checked}
            })
            context.setState(update(context.state, {
                performers: {$splice: [[index, 1, updatedPerformer]]}
            }));
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
                if (performer.id === context.state.selectedPerformerId) {
                    context.unselectPerformer();
                }
            });
        }; 
    }
    getPerformersInCombat() {
        return this.state.performers.filter(p => p.isActive && typeof p.initiative === 'number');
    }
    isActive() {
        return this.state.performers && this.getPerformersInCombat().length > 0;
    }
    startCombat() {
        this.setState({round: 0});
    }
    stopCombat() {
        this.setState(prevState => ({
            round: null,
            performers: prevState.performers.map(p => update(p, {
                initiative: {$set: null}
            }))
        }));
    }
    getPerformerNeedingInitiative() {
        return this.state.performers.find(p => p.isActive && typeof p.initiative !== 'number');
    }
    getFunctionToSetNeededInitiative(context, i) {
        return function(e) {
            var performer = context.getPerformerNeedingInitiative();
            var index = context.state.performers.indexOf(performer);
            if (index < 0) {
                return;
            }
            var updatedPerformer = update(performer, {
                initiative: {$set: i}
            })
            context.setState(update(context.state, {
                performers: {$splice: [[index, 1, updatedPerformer]]}
            }), function() {
                if (context.isActive() && !context.getPerformerNeedingInitiative() && context.state.round === 0) {
                    context.setState({round: 1});
                    // TODO make queue
                }
            });
        }
    }
    render() {
        // One row per Performer
        const performerRows = this.state.performers.map(p => (
            <tr key={p.id} onClick={this.getFunctionToSelectPerformer(this, p)}>
                <td>
                    <input type="checkbox" checked={p.isActive}
                        onChange={this.getFunctionToSetPerformerIsActive(this, p)} />
                </td>
                <td>{p.name}</td>
                <td>{typeof p.initiative === 'number' ? p.initiative : "-"}</td>
                <td>
                    <button onClick={this.getFunctionToRemovePerformer(this, p)}>Remove</button>
                </td>
            </tr>
        ));
        // Possible D20 die rolls
        const rolls = Array.from(new Array(20), (x,i) => i + 1).map(i => (
            <button key={i} onClick={this.getFunctionToSetNeededInitiative(this, i)}>{i}</button>
        ));
        return (
            <div className="App" onClick={this.unselectPerformer}>
                <div className="App-header">
                    <h2>Combat-Q Overview</h2>
                </div>
                {this.state.round > 0 &&
                    <div>Round: {this.state.round}</div>
                }
                {performerRows.length > 0 &&
                    <table>
                        <tbody>
                            <tr>
                                <th title="Is or should be in current combat">Active?</th>
                                <th>Name</th>
                                <th>Initiative</th>
                            </tr>
                            {performerRows}
                        </tbody>
                    </table>
                }
                <button onClick={this.addPerformer}>Add</button>
                {this.isActive() ?
                    <button title="End the current combat" onClick={this.stopCombat}>Stop</button> :
                    <button title="Begin a new combat" onClick={this.startCombat}>Start</button>
                }
                {this.state.selectedPerformerId !== undefined && 
                    <div>
                        <span name="selName">Name:</span>
                        <input type="text" autoFocus
                            value={this.state.selectedPerformerName}
                            onChange={this.handleSelectedPerformerNameInputChange}
                            // Clicking anywhere unselects, but we want to preserve current selection.
                            onClick={this.stopEventPropagation}
                            />
                    </div>
                }
                {this.state.round !== null && this.getPerformerNeedingInitiative() &&
                    <div>
                        <div>
                            Select an initiative roll for: {this.getPerformerNeedingInitiative().name}
                        </div>
                        {rolls}
                    </div>
                }
            </div>
        );
    } // end of render()
}

export default App;
