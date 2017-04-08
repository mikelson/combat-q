//
// Combat-Q, using React JS, main application hook.
//
// Copyright 2017, Peter Mikelsons
//
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
        var performers = this.state.performers;
        var performer = {
            name: "new character",
            id: this.state.nextId
        };
        performers.push(performer);
        // Add new guy, select it, and update the ID counter.
        this.setState(prevState => ({
            performers,
            selectedPerformer: performer,
            selectedPerformerName: performer.name,
            nextId: prevState.nextId + 1
        }));
    }
    unselectPerformer() {
        this.setState(prevState => ({
            selectedPerformerName: "",
            selectedPerformer: undefined
        }));
    }
    handleSelectedPerformerNameInputChange(event) {
        var name = event.target.value;
        var performer = {
            name,
            id: this.state.selectedPerformer.id
        };
        var performers = this.state.performers.map(
            p => p === this.state.selectedPerformer ? performer : p
        );
        this.setState({
            performers,
            selectedPerformer: performer,
            selectedPerformerName: name
        });
    }
    stopEventPropagation(e) {
        e.stopPropagation();
    }
    // should be called with context=this
    getFunctionToSelectPerformer(context, performer) {
        return function(e) {
            // Clicking anywhere unselects, but we want to select performer
            e && e.stopPropagation();
            context.setState(prevState => ({
                selectedPerformer: performer,
                selectedPerformerName: performer.name
            }));
        };
    }
    // should be called with context=this
    getFunctionToRemovePerformer(context, performer) {
        return function(e) {
            context.setState(prevState => ({
                performers: prevState.performers.filter(p => p !== performer)
            }), function() {
                // Don't leave a deleted Performer as selected.
                if (performer === this.state.selectedPerformer) {
                    context.unselectPerformer();
                }
            });
        }; 
    }
    render() {
        const performerRows = this.state.performers.map(p => (
            <tr key={p.id} onClick={this.getFunctionToSelectPerformer(this, p)}>
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
